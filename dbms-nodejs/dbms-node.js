const http = require('http');
const url = require('url');
const fs = require('fs');
const path = require('path');
const mime = require('mime');
//const mysql = require('mysql');
const mariadb = require('mariadb');

const pool = mariadb.createPool({
    host: 'server',
    user: 'user',
    password: 'password',
    database: 'database',
    connectionLimit: 10,
});

function handleDBSearch(uri, res) {
    try {
        console.log("Searching: " + uri.query['q']);

        pool.getConnection().then(conn => {
            conn.query("select * from zipAll where concat(addr1,addr2,addr3) like ?", ("%" + uri.query['q'] + "%"))
                .then(rows => {
                    res.writeHead(200, {
                        'Content-Type': 'text/plain; charset=utf-8'}
                    );

                    if (rows.length == 0) {
                        res.write("No result found!\n");
                    } else {
                        rows.forEach(row => {
                            res.write(JSON.stringify(row) + "\n\n");
                        });
                    }

                    res.end();
                    conn.end();
                })
                .catch(err => {
                    res.writeHead(500, {
                        'Content-Type': 'text/plain'}
                    );
                    res.end("500 Internal Server Error\n");
                    conn.end();
                });
        })
    } catch (err) {
        console.log("missing query param");
        res.writeHead(400, {
            'Content-Type': 'text/plain'}
        );
        res.end("400 Bad Request: missing parameter: \'q\'\n");
    }
}

function handleGetMethod(pathname, uri, req, res) {
    if (uri.pathname == "/db/search") {
        handleDBSearch(uri, res);
        return;
    }

    try {
        var stat = fs.lstatSync(pathname);

        /* if path is a directory then redirect to index.html */
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
    var url_parsed = url.parse(req.url, true);
    var pathname = "html" + url_parsed.pathname;

    console.log(url_parsed);

    if (req.method == "GET") {
        handleGetMethod(pathname, url_parsed, req, res);
    }
}).listen(8080, '0.0.0.0');
console.log('Server running at http://0.0.0.0:8080/');
