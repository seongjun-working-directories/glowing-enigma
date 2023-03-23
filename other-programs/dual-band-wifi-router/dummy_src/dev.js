/*
const puppeteer = require('puppeteer');
const xlsx = require('json-as-xlsx');
// const fs = require('fs');

const crawler = async () => {
  try {
    const browser = await puppeteer.launch({
      headless: process.env.NODE_ENV === 'production',
      args: [
        '--window-size=1920,1080', '--disable-notifications', '--no-sandbox'
      ]
    });

    let results = [];

    for (let v=1; v<=25; v++) {
    // await Promise.all([1,2,3,4,5,6,7,8,9,10].map(async (v)=>{
      const page = await browser.newPage();

      await page.setViewport({
        width: 1920,
        height: 1080,
      });

      const keyword = 'dual+band+wifi+router';  // 키워드 입력
      
      await page.goto(`https://www.amazon.com/s?k=${keyword}&i=computers&rh=n%3A300189%2Cp_n_feature_three_browse-bin%3A23760606011&page=${v}`, {
          waitUntil: `networkidle0`
      });

      results = results.concat(await page.evaluate(()=>{
        const tags = document.querySelectorAll('div.s-main-slot.s-result-list.s-search-results.sg-row > div');
        const result = [];
        tags.forEach(tag=>{
          if (result.length === 6) {
            // Do nothing!
          }
          else {
            const obj = {
              name: tag.querySelector('.a-section.a-spacing-none.puis-padding-right-small.s-title-instructions-style > h2')
                && tag.querySelector('.a-section.a-spacing-none.puis-padding-right-small.s-title-instructions-style > h2').textContent,
              price: tag.querySelector('.a-price') && tag.querySelector('.a-price').textContent,
              link: tag.querySelector('.a-section.a-spacing-none.puis-padding-right-small.s-title-instructions-style > h2 > a')
                && tag.querySelector('.a-section.a-spacing-none.puis-padding-right-small.s-title-instructions-style > h2 > a').href
            }
            if (obj.price !== null) {
              obj.price = `$${obj.price.split('$')[1]}`
            }

            if (obj.name !== null) {
              result.push(obj);
            }
          }
        });
        return result;
      }));
    // ));
    }

    // [TEST] console.log(results);


    results.forEach(async (elem)=>{
      await page.goto(elem.link);
      const properties = await page.evaluate(()=>{
        const brand = document.querySelector('.po-brand .a-span9 .a-size-base')
          && document.querySelector('.po-brand .a-span9 .a-size-base').textContent;
        const modelName = document.querySelector('.po-model_name .a-span9 .a-size-base')
          && document.querySelector('.po-model_name .a-span9 .a-size-base').textContent;
        const productDimensions = document.querySelector('#productDetails_detailBullets_sections1 > tbody > tr:nth-child(1) > td')
          && document.querySelector('#productDetails_detailBullets_sections1 > tbody > tr:nth-child(1) > td').textContent;
        const productWeight = document.querySelector('#productDetails_detailBullets_sections1 > tbody > tr:nth-child(2) > td')
          && document.querySelector('#productDetails_detailBullets_sections1 > tbody > tr:nth-child(1) > td').textContent;
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
          brand, modelName, productDimensions, productWeight,
          frequencyBandClass, wirelessCommunicationStandard, frequency,
          recommendedUsesForProduct, includedComponents,
          connectivityTechnology, color, antennaType,
        };
      });
      elem.brand = properties.brand;
      elem.modelName = properties.modelName;
      elem.productDimensions = properties.productDimensions;
      elem.productWeight = properties.productWeight;
      elem.frequencyBandClass = properties.frequencyBandClass;
      elem.wirelessCommunicationStandard = properties.wirelessCommunicationStandard;
      elem.frequency = properties.frequency;
      elem.recommendedUsesForProduct = properties.recommendedUsesForProduct;
      elem.includedComponents = properties.includedComponents;
      elem.connectivityTechnology = properties.connectivityTechnology;
      elem.color = properties.color;
      elem.antennaType = properties.antennaType;
    });

    await page.close();

    // [TEST]
    console.log(results);

    let data = [
      {
        sheet: 'Amazon Dual Band Wifi Router',
        columns: [
          {label: '상품명', value: 'name'},
          {label: '가격', value: 'price'},
          {label: '링크', value: 'link'},
          {label: '브랜드', value: 'brand'},
          {label: '모델명', value: 'modelName'},
          {label: '주파수 대역 등급', value: 'frequencyBandClass'},
          {label: '무선 통신 표준', value: 'wirelessCommunicationStandard'},
          {label: '주파수', value: 'frequency'},
          {label: '추천 사용 방식', value: 'recommendedUsesForProduct'},
          {label: '포함된 구성요소', value: 'includedComponents'},
          {label: '연결 기술', value: 'connectivityTechnology'},
          {label: '색상', value: 'color'},
          {label: '안테나 타입', value: 'antennaType'},
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

    xlsx(data, settings);

    await browser.close();
  }
  catch (err) {
    console.error(err);
  }
};

const searcher = async () => {
  const result = await crawler();
  // [TEST]
  console.log(result);
}

searcher();
*/