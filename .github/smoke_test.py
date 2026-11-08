"""Check that the WebAssembly game starts in a browser and reacts to clicks."""
import base64
import sys
import time

from playwright.sync_api import sync_playwright

URL = "http://localhost:8000/k3_op_een_rij.html"

with sync_playwright() as p:
    browser = p.chromium.launch(channel="chrome")
    # The same size as the iframe on the website
    page = browser.new_page(viewport={"width": 800, "height": 520})
    errors = []
    page.on("pageerror", lambda e: errors.append(str(e)))
    page.goto(URL)
    time.sleep(10)
    before = page.screenshot()
    after = before
    # Click around until something happens, e.g. 'Begin spel' is clicked.
    # Stop then, as more clicks may bring the game back to the menu
    for x in range(20, 800, 40):
        for y in range(20, 520, 40):
            page.mouse.click(x, y)
            time.sleep(0.1)
            after = page.screenshot()
            if after != before:
                break
        if after != before:
            break
    browser.close()

print("Screenshot:", base64.b64encode(after).decode())
if errors:
    sys.exit(f"Page errors: {errors}")
if before == after:
    sys.exit("Clicking did not change the page")
