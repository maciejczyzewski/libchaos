#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
The script that summarizes/builds the project. (html, markdown)

Usage:
  build.py <path>
  build.py -h | --help
  build.py --version

Options:
  -h --help        Show this screen.
  --version        Show version.
"""

# TODO: Remove spaghetti code from here! More comments and spaces...
# TODO: Better "Algorithms" section... maybe cookbooks in future?
# TODO: Add "Books" or "Articles" to bookmarks...

__version__ = "1.0.0"
__copyright__ = 'Copyright 2015 Maciej A. Czyzewski'

###############################################################################
# IMPORT
###############################################################################

from docopt import docopt
from jinja2 import Template
from unicodedata import normalize

import re
import os
import yaml
import time
import glob

###############################################################################
# FUNCTIONS
###############################################################################

FILENAME = 'RETTER.html'


class Generator:
    def __init__(self, path):
        self.data = {}
        self.storage = os.path.abspath(path) + '/'
        self.templates = os.path.dirname(os.path.realpath(__file__)) + '/'
        self.explore()

    def parse(self, filename):
        if os.path.isfile(filename):
            stream = file(filename, 'r')
        if os.path.isfile(self.storage + filename):
            stream = file(self.storage + filename, 'r')
        return yaml.load(stream)

    def collection(self, name):
        array = []
        for filename in glob.glob(self.storage + 'bookmarks/' + name + '/*'):
            array.append(self.parse(filename))
        return array

    def explore(self):
        self.data['helpers'] = {}
        self.data['algorithms'], self.data['bookmarks'] = {}, {}

        self.data['helpers']['date'] = time.strftime("%d/%m/%Y")
        self.data['helpers']['libraries'] = ['C', 'C++', 'C# / .NET', \
            'Python', 'Javascript', 'Clojure', 'ActionScript', 'Ruby',\
            'Objective-c', 'Rust', 'Haskell', 'Java']
        self.data['helpers']['tools'] = [
            'Penetration Distributions & Frameworks', 'Network',      \
            'SSL Analysis', 'Crackers', 'DoS & DDoS', 'Anonimity',    \
            'Reverse Engineering', 'Fuzzing', 'Forensics',            \
            'Malware & Vulnerability Analysis', 'Others']
        self.data['helpers']['meetups'] = ['major', 'meeting', 'workshop']

        self.data['bookmarks']['libraries'] = self.collection('libraries')
        self.data['bookmarks']['tools'] = self.collection('tools')
        self.data['bookmarks']['courses'] = self.collection('courses')
        self.data['bookmarks']['groups'] = self.collection('groups')
        self.data['bookmarks']['meetups'] = self.collection('meetups')
        self.data['bookmarks']['people'] = self.collection('people')

        self.data['helpers']['primitives'] = {                        \
            'Unkeyed Primitives': [                                   \
                'Hash Functions (MDCs)',                              \
                'One-Way Permutations',                               \
                'Random Sequences',                                   \
            ],                                                        \
            'Symmetric-Key Primitives': [                             \
                ['Symmetric-Key Ciphers',                             \
                    ['Block Ciphers', 'Stream Ciphers']],             \
                'Hash Functions (MACs)',                              \
                'Signatures',                                         \
                'Pseudorandom Sequences',                             \
                'Identification Primitives',                          \
            ],                                                        \
            'Public-Key Primitives': [                                \
                'Public-Key Ciphers',                                 \
                'Signatures',                                         \
                'Identification Primitives',                          \
            ],                                                        \
        }

        self.data['helpers']['taxonomy'] = [
            'Checksums',
            'Cyclic Redundancy Checks',
            'Hash Functions (MDCs)',
            'Hash Functions (MACs)',
            'Block Ciphers',
            'Stream Ciphers',
            'Public-Key Ciphers',
            'Pseudorandom Generators',
            'Pseudorandom Function Family',
        ]

        array = []
        for filename in glob.glob(self.storage + 'algorithms/*/README'):
            print("FILENAME: {0}".format(filename))
            data = self.parse(filename)
            data['labels'] = data['labels'].split(', ')
            data['id'] = slugify(data['name'])
            array.append(data)
            print(data)
        self.data['algorithms'] = array

    def build(self):
        filename = self.templates + '/template.html'
        template = Template(open(filename, 'r').read())
        with open(self.storage + FILENAME, 'w') as stream:
            stream.write(template.render(make_str_unicode(self.data)).encode("utf-8"))

###############################################################################
# MAIN
###############################################################################

_punct_re = re.compile(r'[\t !"#$%&\'()*\-/<=>?@\[\\\]^_`{|},.]+')

def slugify(text, delim=u'-'):
    """Generates an slightly worse ASCII-only slug."""
    result = []
    for word in _punct_re.split(text.lower()):
        word = normalize('NFKD', make_str_unicode(word)).encode('ascii', 'ignore')
        if word:
            result.append(word)
    return unicode(delim.join(result))

def make_str_unicode(obj):
    t = type(obj)

    if t in (list, tuple):
        if t == tuple:
            is_tuple = True
            obj = list(obj)
        else:
            obj = obj[:]
            is_tuple = False

        for x in xrange(len(obj)):
            obj[x] = make_str_unicode(obj[x])

        if is_tuple:
            obj = tuple(obj)

    elif t == dict:
        for k in obj:
            if type(k) == str:
                k = unicode(k)
            obj[k] = make_str_unicode(obj[k])

    elif t == str:
        obj = unicode(obj, 'utf-8')
    return obj

if __name__ == '__main__':
    arguments = docopt(__doc__, version=__version__)

    records = Generator(arguments['<path>'])
    records.build()
