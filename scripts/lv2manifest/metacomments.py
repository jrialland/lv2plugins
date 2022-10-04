import re
from typing import TextIO, Union
import os
import glob
import json
from urllib.parse import urlparse
import logging

# a regular expression that parses comments in C++ files
ccomment = re.compile(r'''(?x)
(?=["'/])      # trick to make it faster, a kind of anchor
(?:
    "(?=((?:[^"\\?]+|\?(?!\?/)|(?:\?\?/|\\)[\s\S])*))\1" # double quotes string
  |
    '(?=((?:[^'\\?]+|\?(?!\?/)|(?:\?\?/|\\)[\s\S])*))\2' # single quotes string
  |
    (
        /(?:(?:\?\?/|\\)\n)*/(?:.*(?:\?\?|\\)/\n)*.* # single line comment
      |
        /(?:(?:\?\?/|\\)\n)*\*                       # multiline comment
        (?=((?:[^*]+|\*+(?!(?:(?:\?\?/|\\)\n)*/))*))\4
        \*(?:(?:\?\?/|\\)\n)*/             
    )
)
''')


def get_metacomment(fd: Union[str, TextIO]) -> Union[str, None]:
    if type(fd) == str:
        with open(str(fd)) as f:
            filedata = f.read()
    else:
        filedata = fd.read()  # type:ignore
    comments = [m[2].strip() for m in ccomment.findall(filedata) if m[2]]
    comments = [s for c in comments for s in c.split('\n')]
    comments = ''.join([re.sub(r'^\*', '', s.strip()) for s in filter(
        lambda s: not (s.startswith('//') or s.startswith('/*')), comments)])
    meta = re.search('@lv2cpp\\.meta {(.+)}', comments)
    return '{' + meta[1] + '}' if meta else None


def scan_directory(dirname: str):
    """ lookup in c++ code, looking for special comments that contain the module's metadata"""
    infos = {}
    for filename in glob.glob(os.path.join(dirname, '**/*.hpp')):
        smeta = get_metacomment(filename)
        if smeta:
            meta = json.loads(smeta)
            meta['__file__'] = filename
            uri = meta['uri']
            infos[uri] = meta
            if not 'name' in meta:
                meta['name'] = os.path.basename(
                    urlparse(meta['uri'].replace('#', '-')).path)
            meta['name'] = meta['name'].lower()
            logging.debug(meta)
    return infos
