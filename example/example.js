const { text_to_accent_phrases } = require("../prebuild/node-v131-napi-v5-linux-x64-glibc-2.39/addon");
const path = require("path");
const { promises: fs, readFileSync } = require("fs");
const path_to_htsvoice = path.resolve(__dirname, "../", "hts_voice_nitech_jp_atr503_m001-1.05", "nitech_jp_atr503_m001.htsvoice");
const htsvoice = readFileSync(path_to_htsvoice).buffer;

(async () => {

  text_to_accent_phrases((err, njd_features) => { console.log("結果:" + JSON.stringify(njd_features)); }, "呪術廻戦竹やぶwhereはWHEREで焼×けたいやaardvarkやUnity。", {
    htsvoice,
    dictionary: await readDictionary(path.resolve(__dirname, "../", "prebuild", "node-v131-napi-v5-linux-x64-glibc-2.39", "dictionary"))
  });
})();


/**
 * 
 * @param {DataView} view 
 * @param {import("node-openjtalk-binding").WaveObject} wave
 */
function createWAV(view, wave) {
  const blockSize = wave.numChannels * wave.bitDepth / 8;
  view.setUint32(0, 0x52494646);//"RIFF"
  view.setUint32(4, wave.data.byteLength + 44 - 8, true);
  view.setUint32(8, 0x57415645);//"WAVE"
  view.setUint32(12, 0x666D7420);//"fmt "
  view.setUint32(16, 16, true);//16(LE)
  view.setUint16(20, 1, true);//LINEAR PCM
  view.setUint16(22, wave.numChannels, true);
  view.setUint32(24, wave.sampleRate, true);
  view.setUint32(28, wave.sampleRate * blockSize, true);
  view.setUint16(32, blockSize, true);
  view.setUint16(34, wave.bitDepth, true);
  view.setUint32(36, 0x64617461);//"data"
  view.setUint32(40, wave.data.byteLength, true);
  let i = 44;
  for (const x of wave.data) {
    view.setInt16(i, x, true);
    i += 2;
  }
}

async function readDictionary(path_to_dictionary) {
  const [unkdic, sysdic, property, matrix] = (await Promise.all(
    [
      readFileSync(path.resolve(path_to_dictionary, "unk.dic")),
      readFileSync(path.resolve(path_to_dictionary, "sys.dic")),
      readFileSync(path.resolve(path_to_dictionary, "char.bin")),
      readFileSync(path.resolve(path_to_dictionary, "matrix.bin"))
    ]
  )).map(e => e.buffer);
  return {
    unkdic,
    sysdic,
    property,
    matrix
  };
}