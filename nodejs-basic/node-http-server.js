const http = require('http');
const url = require('url');
const fs = require('fs');
const path = require('path');
const mime = require('mime');

function handleGetMethod(pathname, uri, req, res) {
    console.log('[*] get: %s %s', pathname, uri.pathname);
    try {
        var stat = fs.lstatSync(pathname);
        if (stat.isDirectory()) {
            res.writeHead(301, {
                'Content-Type': 'text/plain',
                'Location': uri.pathname.substring(1) + 'index.html'
            });
            res.end("301 Redirect\n");
            return;
        }

        fs.readFile(pathname, (err, data) => {
            res.writeHead(200, {
                'Content-Type': mime.getType(pathname)
            });
            res.write(data);
            res.end();
        });
    } catch (err) {
        res.writeHead(404, {
            'Content-Type': 'text/plain'}
        );
        res.end("404 Not Found\n");
    }
}

http.createServer(function (req, res) {
    var url_parsed = url.parse(req.url);
    var pathname = "html" + url_parsed.pathname;

    if (req.method == "GET") {
        handleGetMethod(pathname, url_parsed, req, res);
    }
}).listen(8080, '0.0.0.0');
console.log('Server running at http://0.0.0.0:8080/');
