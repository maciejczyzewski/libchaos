# -*- coding: utf-8 -*-

# == BASICS ====================================================================

project = u'libchaos'
release = u'0.0.1-dev'
author = u'Maciej A. Czyzewski'

# == SPHINX ====================================================================

extensions = ['sphinx.ext.todo','sphinx.ext.mathjax','sphinx.ext.githubpages']
templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'index'
copyright = u'2016, Libchaos Authors'
version = release
language = None
exclude_patterns = []
pygments_style = 'friendly'
todo_include_todos = True
html_theme = "_assets"
html_theme_path = ["."]
html_sidebars = {'index':['about.html', 'sourcelink.html', 'searchbox.html'],'search':['about.html'],'**':['localtoc.html', 'relations.html', 'sourcelink.html', 'searchbox.html']}
html_logo = 'libchaos.svg'
html_static_path = ['_static']
html_use_smartypants = True
html_use_opensearch = 'True'
htmlhelp_basename = 'libchaosdoc'
