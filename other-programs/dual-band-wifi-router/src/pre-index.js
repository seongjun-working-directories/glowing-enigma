const puppeteer = require('puppeteer');
const xlsx = require('json-as-xlsx');
const fs = require('fs');

const crawler = async () => {
  try {
    const browser = await puppeteer.launch({
      headless: process.env.NODE_ENV === 'production',
      args: [
        '--window-size=1920,1080', '--disable-notifications', '--no-sandbox'
      ]
    });

    let results = [];

    const page = await browser.newPage();
    await page.setViewport({ width: 1920, height: 1080, });

    for (let v=1; v<=25; v++) {
      const keyword = 'dual+band+wifi+router';  // 키워드 입력

      await page.goto(`https://www.amazon.com/s?k=${keyword}&i=computers&rh=n%3A300189%2Cp_n_feature_three_browse-bin%3A23760606011&page=${v}`, {
          waitUntil: `networkidle2`
      });

      results = results.concat(await page.evaluate(()=>{
        const tags = document.querySelectorAll('div.s-main-slot.s-result-list.s-search-results.sg-row > div');
        const result = [];
        tags.forEach((tag) => {
          if (result.length === 6) { /* Do nothing! */ }
          else {
            const obj = {
              name: tag.querySelector('.a-section.a-spacing-none.puis-padding-right-small.s-title-instructions-style > h2')
                && tag.querySelector('.a-section.a-spacing-none.puis-padding-right-small.s-title-instructions-style > h2').textContent,
              price: tag.querySelector('.a-price') && tag.querySelector('.a-price').textContent,
              link: tag.querySelector('.a-section.a-spacing-none.puis-padding-right-small.s-title-instructions-style > h2 > a')
                && tag.querySelector('.a-section.a-spacing-none.puis-padding-right-small.s-title-instructions-style > h2 > a').href
            }
            if (obj.price !== null) { obj.price = `$${obj.price.split('$')[1]}`; }
            if (obj.name !== null) { result.push(obj); }
          }
        });
        return result;
      }));
    }

    await page.close();

    let data = [
      {
        sheet: 'Amazon Dual Band Wifi Router',
        columns: [
          {label: 'name', value: 'name'},
          {label: 'price', value: 'price'},
          {label: 'link', value: 'link'}
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

crawler();