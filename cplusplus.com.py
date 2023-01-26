#!/usr/bin/env python3

import enum
import hashlib
import json
import logging
import os

import lxml.html
import urllib.parse
import urllib.request

logging.basicConfig(level=logging.INFO)


class Cache:
  def __init__(self, dir:str):
    os.makedirs(dir, exist_ok=True)
    self.dir = dir

  def _get_cache_path(self, url:str):
    h = hashlib.sha1()
    h.update(url.encode('utf-8'))
    return os.path.join(self.dir, h.hexdigest() + '.json')

  def get_entry(self, url:str):
    return CacheEntry(self._get_cache_path(url), url)


class NoCache:
  def __init__(self):
    self.data = { 'content' : None }
    self.outdated = True

  def save(self):
    pass

  def load(self):
    pass

  def __getitem__(self, key):
    return self.data[key]

  def __setitem__(self, key, value):
    self.data[key] = value
    self.outdated = True


class CacheEntry(NoCache):
  def __init__(self, path, url):
    self.path = path
    self.url  = url
    self.data = None

  def load(self):
    if self.data == None and os.path.exists(self.path):
      with open(self.path, 'r') as infile:
        logging.info(f'loading cache file {self.path}, url={self.url}')
        self.data = json.load(infile)
        self.outdated = False
    else:
      logging.info(f'initalizing empty cache, url={self.url}')
      self.data = { 'url' : self.url, 'content' : None }

  def save(self):
    if self.outdated:
      with open(self.path, 'w') as outfile:
        logging.info(f'saving cache file {self.path}, url={self.url}')
        json.dump( self.data, outfile )
        self.outdated = False


class Page:
  def __init__(self, url, cache=None):
    self.url = url
    if (cache == None):
      cache = NoCache()
    self._cache = cache
    self._tree = None

  def __enter__(self):
    self._cache.load()
    self._open();
    return self

  def __exit__(self, type, value, traceback):
    self._cache.save()

  def _parse_tree(self):
    if self._tree == None:
      self._tree = lxml.html.fromstring(self._cache['content'])

  def _open(self, force=False):
    if force or self._cache['content'] == None:
      logging.info(f'open HTTP request to url={self.url}')
      with urllib.request.urlopen(self.url) as response:
        # TODO: parse encoding type from the response, assume utf8 for now
        content = response.read().decode('utf-8')
        self._cache['content'] = content

  def xpath(self, xpath):
    self._parse_tree()
    return self._tree.xpath( xpath )


class Spider:
  def __init__(self, starturl, depth=1, cache:Cache=None):
    self.starturl = starturl
    self.depth = depth
    self.cache = cache

  def get_cache_entry(self, url:str):
    if self.cache != None and url != None:
      return self.cache.get_entry(url)
    return None

  def child_urls(self, page:Page):
    return [a.attrib['href'] for a in page.xpath('//a')]

  def crawl(self):
    return CrawlPath(self, self.starturl, self.depth)


class CrawlPath:
  def __init__(self, spider:Spider, starturl, depth):
    self._spider = spider
    self._maxdepth = depth
    self._seen = set()
    self._previouspage = None
    self._currentdepth = 0
    self._currentlevel = []
    self._nextlevel = [starturl]

  def _exit_previouspage(self):
    pp = self._previouspage
    if pp != None:
      for child in self._spider.child_urls(pp):
        self._nextlevel.append( urllib.parse.urljoin(pp.url, child) )
      pp.__exit__(None, None, None)
    self._previouspage = None

  def _enter_nextpage(self, url):
    self._seen.add(url)
    self._previouspage = Page(url, cache=self._spider.get_cache_entry(url))
    self._previouspage.__enter__();
    return self._previouspage

  def _pop_next(self):
    if not self._currentlevel:
      self._currentlevel = self._nextlevel
      self._nextlevel = []
      self._currentdepth += 1
      logging.debug(f'crawl: Iterating to level (depth = {self._currentdepth})')

    if self._currentdepth > self._maxdepth:
      logging.debug(f'crawl: Stopped. Reached maxdepth={self._maxdepth}')
      return None

    if self._currentlevel:
      return self._currentlevel.pop(0)

    logging.debug(f'crawl: No more unseen URLs (stopped at depth = {self._currentdepth})')
    return None

  def __iter__(self):
    return self

  def __next__(self):
    self._exit_previouspage()

    # Get the next URL (skip seen):
    url = self._pop_next()
    while url != None and url in self._seen:
      logging.debug(f'crawl: Skipping seen {url}')
      url = self._pop_next()

    if url != None:
      return self._enter_nextpage(url)
    raise StopIteration


class CplusplusDotComStringSpider(Spider):

  def child_urls(self, page:Page):
    return list(filter(
      lambda x: x.startswith ('/reference/string/string'),
      (a.attrib['href'] for a in page.xpath('//a'))
    ))

def example_name(url):
  # Get the URL base name ignoring any trailing slashes
  basename = os.path.basename(os.path.dirname(url+'/'))
  # Replace characters that are invalid for CMake target names
  # TODO find & replace stuff here in basename
  substitutions = {
    'operator+=' : 'operator_plusequal',
    'operator+'  : 'operator_plus',
    'operator='  : 'operator_equal',
    'operator[]' : 'operator_squarebrackets'
  }
  for k,v in substitutions.items():
    basename = basename.replace(k, v)
  return 'string_' + basename

def main():
  spider = CplusplusDotComStringSpider(
    starturl = 'http://cplusplus.com/reference/string/string',
    depth = 5,
    cache = Cache('out/cache')
  )
  for page in spider.crawl():
    source = page.xpath("//td[@class='source']")
    output = page.xpath("//td[@class='output']")
    assert len(source) in [0,1]
    assert len(output) in [0,1]
    if len(source) == 1 and len(output) == 1:
      lines = [ '[URL]', page.url, '',
                '[Source]', source[0].text_content(), '',
                '[Output]', output[0].text_content()]
      path = os.path.join('out', example_name(page.url) + '.cpp.in')
      with open(path, 'w') as f:
        logging.info(f'writing file: {path}')
        f.write('\n'.join(lines))

if __name__ == '__main__':
  main()
