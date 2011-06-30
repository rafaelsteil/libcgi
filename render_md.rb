#!/usr/bin/ruby
# Markdown sintax at http://daringfireball.net/projects/markdown/syntax
# Depends on https://github.com/tanoku/redcarpet

require 'rubygems'
require 'redcarpet'

file = 'README.md'
markdown = Redcarpet.new(File.read(file), :autolink, :generate_toc)

File.open('README.html', 'w') { |f| f.write(markdown.to_html) }