#!/usr/bin/env ruby
if ARGV.count != 1
  exit
end

words = ARGV[0].gsub('_', "\\_")
dir = File.expand_path(File.dirname(__FILE__))
open(File.join(dir, 'useful-english.md'), 'a+') do |f|
  f.puts("- " + words)
  f.puts
end

