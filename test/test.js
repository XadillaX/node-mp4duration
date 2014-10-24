/**
 * XadillaX created at 2014-10-24 19:01:01
 *
 * Copyright (c) 2014 Huaban.com, all rights
 * reserved
 */
var fs = require("fs");
var parser = require("../");

var filename = "test/small.mp4";
var buff = fs.readFileSync(filename);

parser.parse(buff, function(err, d) { console.log(d); });
parser.parse(filename, function(err, d) { console.log(d); });
console.log(parser.parseSync(buff));
console.log(parser.parseSync(filename));
parser.parse("binding.gyp", function(err, d) { console.log(err.message); });

