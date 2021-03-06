/**
 * XadillaX created at 2014-10-24 16:27:19
 *
 * Copyright (c) 2014 Huaban.com, all rights
 * reserved
 */
var fs = require("fs");
var parser = require("./build/Release/mp4duration.node");

/**
 * parse sync
 * @param {string|buffer} filename the filename or buffer of the mp4 file
 */
exports.parseSync = function(filename) {
    if(Buffer.isBuffer(filename)) {
        return parser.parseByBuffer(filename);
    }

    return parser.parseByFilename(filename);
};

/**
 * parse
 * @param {string|buffer} filename the filename or buffer of the mp4 file
 * @param {function} callback the callback function
 */
exports.parse = function(filename, callback) {
    if(Buffer.isBuffer(filename)) {
        var parseBuff = function() {
            var result;
            try {
                result = parser.parseByBuffer(filename);
            } catch(e) {
                return callback(e);
            }

            return callback(undefined, result);
        };

        return process.nextTick(parseBuff);
    }

    fs.readFile(filename, function(err, buff) {
        if(err) return callback(err);

        var result;
        try {
            result = parser.parseByBuffer(buff);
        } catch(e) {
            return callback(e);
        }

        return callback(undefined, result);
    });
};

