import requests
import re
import os
import time

def read_words():
    with open('words.txt', 'r') as file:
        words = [line.strip() for line in file.readlines()]
    return words

def getHTMLText(url):
    try:
        r = requests.get(url, timeout=30)
        r.raise_for_status()
        r.encoding = r.apparent_encoding
        return r.text
    except:
        return ""
    
def get_synonyms(html):
    pos = html.find('<div id="synoid"')
    if pos == -1:
        return []
    area = html[pos:pos + max(10000, len(html) - pos)]
    words = re.findall(
        r'<span[^>]*class="[^"]*p1-4[^"]*"[^>]*>(.*?)</span>',
        area,
        re.S
    )
    res = []
    for word in words:
        word = re.sub(r'<.*?>', '', word)
        word = word.strip()
        if word and word not in res:
            res.append(word)
    return res

def get_image_url(html):
    html = html.replace("&amp;", "&")
    html = html.replace("\\u0026", "&")
    html = html.replace("\\/", "/")
    pattern = r'https?://(?:ts\d+|tse\d*)\.mm\.bing\.net/[^"\'<>\s]+'
    match = re.search(pattern, html)
    if match:
        return match.group(0)
    return ""

def download_image(url, word):
    try:
        r = requests.get(url, timeout=30)
        r.raise_for_status()
        filename = f'{word}.png'
        with open(filename, 'wb') as f:
            f.write(r.content)
    except:
        print(f"Failed to download image for word: {word}")

words = read_words()
synonyms_dict = {}

for word in words:
    url = f'https://cn.bing.com/dict/search?q={word}'
    html = getHTMLText(url)
    synonyms = get_synonyms(html)
    synonyms_dict[word] = synonyms
    img_url = get_image_url(html)
    if img_url:
        download_image(img_url, word)
    # else:
    #     print(f"No image found for word: {word}")
    time.sleep(0.5)

with open('result.txt', 'w') as f:
    for word, synonyms in synonyms_dict.items():
        f.write(f'${word}\n')
        for synonym in synonyms:
            f.write(f'{synonym}\n')