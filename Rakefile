# -*- ruby -*-
# Rakefile for building Octave-ruby Gem and tarball

require 'rubygems/package_task'
require 'rake/packagetask'

require './lib/octave/version.rb'

task :default => [:package]

spec = Gem::Specification.new do |spec|
  spec.name = 'octave-ruby'
  spec.version = Octave::Version::STRING
  spec.summary = "A Ruby interface to the Octave interpreted language."
  spec.description = 'A Ruby interface to the Octave interpreted language.'

  spec.author = ["Jonathan Younger"]
  spec.email = ["jonathan@daikini.com"]
  spec.homepage = "https://github.com/daikini/octave-ruby"
  spec.licenses = ['GPLv3']

  spec.required_ruby_version = '>= 2.0.0'
  spec.requirements = [ 'GNU Octave library and header files' ]

  spec.files = FileList['ext/octave_api/*.cpp',
                   'ext/octave_api/*.c',
                   'ext/octave_api/*.h',
                   'lib/octave.rb',
                   'lib/octave',
                   'test',
                   'setup.rb',
                   'LICENSE.txt',
                   'Manifest.txt',
                   'README.txt'
                   ]
  spec.extensions = Dir['ext/octave_api/extconf.rb']
end

Gem::PackageTask.new(spec) do |pkg|
  pkg.need_zip = false
  pkg.need_tar = true
end


# vim: syntax=Ruby
