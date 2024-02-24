const fs = require("fs");
const crypto = require("crypto");

// Função para calcular o hash SHA256 de um arquivo
function calculateHash(filePath) {
  const data = fs.readFileSync(filePath);
  const hash = crypto.createHash("sha256");
  hash.update(data);
  return hash.digest();
}

// Função para converter um array de bytes para uma string hexadecimal
function bytesToHex(bytes) {
  return bytes.map((byte) => byte.toString(16).padStart(2, "0")).join(", 0x");
}

// Caminho para o arquivo .exe
const exeFilePath =
  "E:/Games/GTA San Andreas - Definitive Edition/Gameface/Binaries/Win64/SanAndreas.exe";

// Calcular o hash do arquivo
const fileHash = calculateHash(exeFilePath);

// Tamanho do arquivo
const fileSize = fs.statSync(exeFilePath).size;

// Gerar o código C++
const cppHash = `FileHash{0x${bytesToHex(fileHash)}}`;
const cppSize = `SIZE_GTA_EXE{0x${fileSize.toString(16)}}`;

// Imprimir o resultado
console.log(`static constexpr ${cppHash};`);
console.log(`static constexpr ${cppSize};`);
