const fs = require("fs");
const readline = require("readline");

const fname = "mecab-ipadic-neologd.csv";
const tempFname = "temp.csv";

const rl = readline.createInterface({
  input: fs.createReadStream(fname),
  output: fs.createWriteStream(tempFname),
  terminal: false
});

rl.on("line", (line) => {
  rl.output.write(line.toLowerCase() + "\n");
});

rl.on("close", () => {
  fs.renameSync(tempFname, fname);
});