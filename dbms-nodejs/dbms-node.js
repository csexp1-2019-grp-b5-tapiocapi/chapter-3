const https = require('https');
const url = require('url');
const fs = require('fs');
const path = require('path');
const mime = require('mime');
const mariadb = require('mariadb');

const tls_key = "key.pem";
const tls_cert = "cert.pem";

const pool = mariadb.createPool({
    host: '127.0.0.1',
    user: 'csexp1',
    password: 'password',
    database: 'database',
    connectionLimit: 10,
});

function handleDBSearch(uri, res) {
    try {
        const query = uri.query['q'];
        if (query.length === 0) {
            res.writeHead(200, {
                'Content-Type': 'application/json; charset=utf-8'}
            );
            res.write(JSON.stringify([]));
            res.end();
            return;
        }

        console.log("Searching: " + query);

        pool.getConnection().then(conn => {
            var queryres = null;
            if (!isNaN(query)) {
                queryres = conn.query("select * from zipAll where zip like ?", ("%" + query + "%"));
            } else if (query.match(/^[\u30a0-\u30ff]+$/)) {
                queryres = conn.query("select * from zipAll where concat(kana1,kana2,kana3) like ?", ("%" + query + "%"));
            } else {
                queryres = conn.query("select * from zipAll where concat(addr1,addr2,addr3) like ?", ("%" + query + "%"));
            }

            queryres
                .then(rows => {
                    res.writeHead(200, {
                        'Content-Type': 'application/json; charset=utf-8'}
                    );

                    res.write(JSON.stringify(rows));
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
    if (uri.pathname === "/db/search") {
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

const tls_opts = {
    key: fs.readFileSync(tls_key),
    cert: fs.readFileSync(tls_cert),
};

https.createServer(tls_opts, (req, res) => {
    var url_parsed = url.parse(req.url, true);
    var pathname = "frontend" + url_parsed.pathname;

    console.log(url_parsed);

    if (req.method === "GET") {
        handleGetMethod(pathname, url_parsed, req, res);
    }
}).listen(8443, '0.0.0.0');
console.log('Server running at https://0.0.0.0:8443/');
