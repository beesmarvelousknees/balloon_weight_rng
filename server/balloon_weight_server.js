const https = require('https');
const fs = require('fs');
const url = require('url');
const { execFile } = require('child_process');
const querystring = require('querystring');


// Load configuration file.
function loadConfig(filePath) {
  const config = {};
  const data = fs.readFileSync(filePath, 'utf8');
  const lines = data.split('\n');

  lines.forEach(line => {
    const [key, value] = line.split('=');
    if (key && value) {
      config[key.trim()] = value.trim();
    }
  });
  return config;
}
const config = loadConfig('./config.cfg');
const PORT = config.PORT;


// Options
const options = {
  key: fs.readFileSync('./server.key'),   // Path to your SSL key
  cert: fs.readFileSync('./server.crt'), // Path to your SSL certificate
};


// Routes
const server = https.createServer(options, (req, res) => {
  const parsedUrl = url.parse(req.url);

  if (req.method === 'GET' && parsedUrl.pathname === '/') {
    // Parse the "count" query param; default to 16 if not specified
    const query = querystring.parse(parsedUrl.query);
    const count = query.count || '32';

    // Call the local get_rand_bytes script
    execFile('./get_rand_bytes', [count], (error, stdout, stderr) => {
      if (error) {
        res.writeHead(500, { 'Content-Type': 'text/plain' });
        return res.end(`Failed to get random bytes: ${error.message}`);
      }
      res.writeHead(200, { 'Content-Type': 'text/plain' });
      res.end(stdout);
    });
  } else {
    // Return 404 for other paths
    res.writeHead(404, { 'Content-Type': 'text/plain' });
    res.end('Not Found');
  }
});

/* If you don't use tailscale this is fine.
server.listen(PORT, () => {
  console.log('HTTPS server listening on https://localhost:8443');
});
*/

// I want it accessible over tailscale too.
server.listen(PORT, ['127.0.0.1', '100.68.63.35'], () => {
  console.log('Server listening on Tailscale and Local IP');
});
