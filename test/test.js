/**
 * XadillaX created at 2014-10-24 19:01:01
 *
 * Copyright (c) 2014 Huaban.com, all rights
 * reserved
 */
var should = require("should");
var fs = require("fs");
var parser = require("../");

var MP4 = "test/assets/sample.mp4";
var M4A = "test/assets/sample.m4a";

describe("parse mpeg4 files", function() {
    it("mp4 buffer should be parsed", function(done) {
        var buff = fs.readFileSync(MP4);
        parser.parse(buff, function(err, d) {
            should(err).be.empty;
            d.should.within(5, 6);
            done();
        });
    });

    it("mp4 file should be parsed", function(done) {
        parser.parse(MP4, function(err, d) {
            should(err).be.empty;
            d.should.within(5, 6);
            done();
        });
    });

    it("m4a buffer should be parsed", function(done) {
        var buff = fs.readFileSync(M4A);
        parser.parse(buff, function(err, d) {
            should(err).be.empty;
            d.should.within(4, 5);
            done();
        });
    });

    it("m4a file should be parsed", function(done) {
        parser.parse(M4A, function(err, d) {
            should(err).be.empty;
            d.should.within(4, 5);
            done();
        });
    });

    it("should be ok while parsing sync", function() {
        var buff = fs.readFileSync(MP4);
        parser.parseSync(buff).should.within(5, 6);
        parser.parseSync(MP4).should.within(5, 6);
        buff = fs.readFileSync(M4A);
        parser.parseSync(buff).should.within(4, 5);
        parser.parseSync(M4A).should.within(4, 5);
    });

    it("shouldn't parse", function(done) {
        parser.parse("binding.gyp", function(err) {
            (err instanceof Error).should.be.ok;
            err.message.should.be.eql("Broken MP4 file.");
            done();
        });
    });
});

