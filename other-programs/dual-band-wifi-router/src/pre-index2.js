// 기존 Spreadsheet.xlsx를 Spreadsheet1.xlsx로 변경해야 함
// 또한 해당 파일의 출력물인 Spreadsheet1.json의 양이 많다면, Spreadsheet1과 Spreadsheet2로 분리해야 함

const jsonConverter = require('xlsx-to-json');

jsonConverter({
  input: 'Spreadsheet1.xlsx',
  output: 'Spreadsheet1.json'
}, function(err, result) {
  if (err) {
    console.error(err);
  }
  else {
    console.log(result);
  }
});