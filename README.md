# Node MP4 Duration

[![node-mp4duration](http://img.shields.io/npm/v/mp4duration.svg)](https://www.npmjs.com/package/mp4duration) [![Dependency Status](https://img.shields.io/david/XadillaX/node-mp4duration.svg)](https://david-dm.org/xadillax/node-mp4duration) [![TravisCI](https://img.shields.io/travis/XadillaX/node-mp4duration/master.svg)](https://travis-ci.org/XadillaX/node-mp4duration) [![Coveralls](https://img.shields.io/coveralls/XadillaX/node-mp4duration/master.svg)](https://coveralls.io/r/XadillaX/node-mp4duration)

A node.js package for parse mp4 duration.

## Installation

```shell
$ npm install mp4duration
```

## Usage

First you should require this package:

```javascript
var mp4duration = require("mp4duration");
```

There're two API for you to parse your mp4 file duration:

+ `parse(filename|buffer)`
+ `parseSync(filename|buffer, callback)`

Each API can receive either `String` or `Buffer`. If you passed a `String`, it should be the filename of your mp4 file, otherwise, you should pass your MP4 file buffer.

### Example

#### parse

```javascript
var filename = "test/small.mp4";
var buff = fs.readFileSync(filename);
parser.parse(buff, function(err, d) { console.log(d); });
parser.parse(filename, function(err, d) { console.log(d); });
```

#### parseSync

```javascript
var filename = "test/small.mp4";
var buff = fs.readFileSync(filename);
console.log(parser.parseSync(buff));
console.log(parser.parseSync(filename));
```

## Contribute

You're welcome to make pull requests!

「雖然我覺得不怎麼可能有人會關注我」

