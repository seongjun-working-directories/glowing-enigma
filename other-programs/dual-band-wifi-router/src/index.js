const puppeteer = require('puppeteer');
const xlsx = require('json-as-xlsx');
const fs = require('fs');

// 1.
// json 파일을 json object 변수에 담음
const buildJsonObject = async () => {
  // const result = await JSON.parse(fs.readFileSync('Spreadsheet1.json').toString());
  const result = await JSON.parse(fs.readFileSync('Spreadsheet2.json').toString());
  return result;
};

// 2.
// url을 전달받아 해당 url 내의 내용을 크롤링함
const URLcrawler = async (url) => {
  const browser = await puppeteer.launch({
    // headless: process.env.NODE_ENV === 'production',
    headless: false,
    args: [ '--window-size=1920,1080', '--disable-notification' ],
    ignoreDefaultArgs: [ '--disable-extensions' ],
  });
  const page = await browser.newPage();
  await page.setViewport({ width: 1920, height: 1080, });

  await page.goto(`${url}`, { waitUntil: 'networkidle2' });
  await page.waitForSelector('.po-brand');

  await page.evaluate(()=>{
    window.scrollTo(0, 5000);
    window.scrollTo(0, 10000);
  });

  await delay(2500);

  const result = await page.evaluate(()=>{
    const brand = document.querySelector('.po-brand .a-span9 .a-size-base')
      && document.querySelector('.po-brand .a-span9 .a-size-base').textContent;

    const modelName = document.querySelector('.po-model_name .a-span9 .a-size-base')
      && document.querySelector('.po-model_name .a-span9 .a-size-base').textContent;

    let productDetails = null;
    productDetails = document.querySelector('#prodDetails')
      && document.querySelector('#prodDetails').textContent;

    if (productDetails) {
      productDetails = productDetails.replace(/ +/g, ' ').split("Product information")[1].replace('\n', ' ').split('Customer Reviews')[0];
    }

    const frequencyBandClass = document.querySelector('.po-frequency_band_class .a-span9 .a-size-base')
      && document.querySelector('.po-frequency_band_class .a-span9 .a-size-base').textContent;
    const wirelessCommunicationStandard = document.querySelector('.po-wireless_comm_standard .a-span9 .a-size-base')
      && document.querySelector('.po-wireless_comm_standard .a-span9 .a-size-base').textContent;
    const frequency = document.querySelector('.po-frequency .a-span9 .a-size-base')
      && document.querySelector('.po-frequency .a-span9 .a-size-base').textContent;
    const recommendedUsesForProduct = document.querySelector('.po-recommended_uses_for_product .a-span9 .a-size-base')
      && document.querySelector('.po-recommended_uses_for_product .a-span9 .a-size-base').textContent;
    const includedComponents = document.querySelector('.po-included_components .a-span9 .a-size-base')
      && document.querySelector('.po-included_components .a-span9 .a-size-base').textContent;
    const connectivityTechnology = document.querySelector('.po-connectivity_techonology .a-span9 .a-size-base')
      && document.querySelector('.po-connectivity_techonology .a-span9 .a-size-base').textContent;
    const color = document.querySelector('.po-color .a-span9 .a-size-base')
      && document.querySelector('.po-color .a-span9 .a-size-base').textContent;
    const antennaType = document.querySelector('.po-antenna.type .a-span9 .a-size-base')
      && document.querySelector('.po-antenna.type .a-span9 .a-size-base').textContent;

    return {
      brand, modelName,
      frequencyBandClass, wirelessCommunicationStandard, frequency, recommendedUsesForProduct,
      includedComponents, connectivityTechnology, color, antennaType, productDetails
    };
  });

  await page.close();
  await browser.close();

  // [TEST]
  console.log('RESULT>>>', result);

  return result;
};

// 3.
// xlsx 파일로 추출함
const searcher = async () => {
  const targetArray = [];
  const results = await buildJsonObject();

  for (let i=0; i<results.length; i++) {
    const url = results[i].link.toString();
    try {
      const tmp = await URLcrawler(`${url}`);
      if (tmp.brand === null || tmp.brand === undefined) { i--; continue; }

      results[i].name = results[i].name;
      results[i].price = results[i].price;
      results[i].brand = tmp.brand;
      results[i].modelName = tmp.modelName;
      results[i].frequencyBandClass = tmp.frequencyBandClass;
      results[i].wirelessCommunicationStandard = tmp.wirelessCommunicationStandard;
      results[i].frequency = tmp.frequency;
      results[i].recommendedUsesForProduct = tmp.recommendedUsesForProduct;
      results[i].includedComponents = tmp.includedComponents;
      results[i].connectivityTechnology = tmp.connectivityTechnology;
      results[i].color = tmp.color;
      results[i].antennaType = tmp.antennaType;
      results[i].link = results[i].link;
      results[i].productDetails = tmp.productDetails;

      console.log('TARGET_ARRAY>>>', results[i]);
      targetArray.push(results[i]);
    }
    catch(err) {
      console.log(err);
      console.log(`${results[i].link} ::: Retrying...`);

      if (!results[i].brand) {
        const url = results[i].link.toString();

        try {
          const tmp = await URLcrawler(`${url}`);

          console.log('results[i].modelname>>>', results[i].modelName);
          console.log('tmp.modelname>>>', tmp.modelName);

          results[i].name = results[i].name;
          results[i].price = results[i].price;
          results[i].brand = tmp.brand;
          results[i].modelName = tmp.modelName
          results[i].frequencyBandClass = tmp.frequencyBandClass;
          results[i].wirelessCommunicationStandard = tmp.wirelessCommunicationStandard;
          results[i].frequency = tmp.frequency;
          results[i].recommendedUsesForProduct = tmp.recommendedUsesForProduct;
          results[i].includedComponents = tmp.includedComponents;
          results[i].connectivityTechnology = tmp.connectivityTechnology;
          results[i].color = tmp.color;
          results[i].antennaType = tmp.antennaType;
          results[i].link = link;
          results[i].productDetails = tmp.productDetails;

          targetArray.push(results[i]);

          // [TEST]
          console.log(results[i]);
        }
        catch (err) {
          console.log(err);
          console.log(`${results[i].link} ::: Please check again...`);
          continue;
        }
      }
      else {
        console.log('No Problem~');
      }

    }
  }

  console.log(targetArray);

  let data = [
    {
      sheet: 'Amazon Dual Band Wifi Router',
      columns: [
        {label: 'name', value: 'name'},
        {label: 'price', value: 'price'},
        {label: 'link', value: 'link'},
        {label: 'brand', value: 'brand'},
        {label: 'modelName', value: 'modelName'},
        {label: 'frequencyBandClass', value: 'frequencyBandClass'},
        {label: 'wirelessCommunicationStandard', value: 'wirelessCommunicationStandard'},
        {label: 'frequency', value: 'frequency'},
        {label: 'recommendedUsesForProduct', value: 'recommendedUsesForProduct'},
        {label: 'includedComponents', value: 'includedComponents'},
        {label: 'connectivityTechnology', value: 'connectivityTechnology'},
        {label: 'color', value: 'color'},
        {label: 'antennaType', value: 'antennaType'},
        {label: 'productDetails', value: 'productDetails'}
      ],
      content: results,
    }
  ];

  let settings = {
    filename: 'Amazon+Dual+Band+Wifi+Router',
    extraLength: 4,
    writeMode: 'writeFile',
    writeOptions: {},
    RTL: true,
  }

  await xlsx(data, settings);
};

// 4.
// 대기
function delay(time) {
  return new Promise(function(resolve) { 
      setTimeout(resolve, time)
  });
}

searcher();