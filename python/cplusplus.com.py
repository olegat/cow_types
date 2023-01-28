#!/usr/bin/env python3

import logging
import os

from spiders import Cache, Spider, Page

logging.basicConfig(level=logging.INFO)


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
