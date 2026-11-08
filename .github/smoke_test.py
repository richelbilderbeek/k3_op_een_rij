"""Check that the WebAssembly game works in a browser.

1. The game starts and reacts to clicks
2. Computer players make moves (this broke when the game logic used
   C++ exceptions, which are disabled in Qt for WebAssembly)
"""
import base64
import io
import sys
import time

from PIL import Image
from playwright.sync_api import sync_playwright

URL = "http://localhost:8000/k3_op_een_rij.html"
# The same size as the iframe on the website
VIEWPORT = {"width": 800, "height": 520}


def find_red_square(screenshot):
    """Find the top-left of the red background of the first girl in the menu.

    The menu shows three girls, on a red, blue and green background,
    with a computer icon to the right of each girl.
    """
    image = Image.open(io.BytesIO(screenshot)).convert("RGB")
    width, height = image.size
    pixels = image.load()
    reds = [
        (x, y)
        for y in range(height)
        for x in range(width)
        if pixels[x, y] == (255, 0, 0)
    ]
    if len(reds) < 100:
        sys.exit("Could not find the player selection in the menu")
    return min(x for x, _ in reds), min(y for _, y in reds)


def start_page(browser, errors):
    page = browser.new_page(viewport=VIEWPORT)
    page.on("pageerror", lambda e: errors.append(str(e)))
    page.on("console", lambda m: print("console:", m.text))
    page.goto(URL)
    time.sleep(10)
    return page


def test_clicks(browser, errors):
    """Click around until something happens, e.g. 'Begin spel' is clicked.

    Stop then, as more clicks may bring the game back to the menu
    """
    page = start_page(browser, errors)
    before = page.screenshot()
    after = before
    for x in range(20, VIEWPORT["width"], 40):
        for y in range(20, VIEWPORT["height"], 40):
            page.mouse.click(x, y)
            time.sleep(0.1)
            after = page.screenshot()
            if after != before:
                break
        if after != before:
            break
    print("Screenshot after clicking:", base64.b64encode(after).decode())
    if before == after:
        sys.exit("Clicking did not change the page")
    page.close()


def test_computer_players(browser, errors):
    """Let three computers play and check that they make moves."""
    page = start_page(browser, errors)
    left, top = find_red_square(page.screenshot())
    # Click the computer icons right of the three girls
    for row in range(3):
        page.mouse.click(left + 75, top + 25 + 50 * row)
        time.sleep(0.2)
    # 'Begin spel' is the first button, left of the girls
    page.mouse.click(left - 80, top + 20)
    # A computer moves every 0.1 seconds, so the screen keeps changing,
    # until the game ends
    screenshots = []
    for _ in range(15):
        time.sleep(0.2)
        screenshots.append(page.screenshot())
    print("Screenshot of computers playing:", base64.b64encode(screenshots[4]).decode())
    n_different = len(set(screenshots))
    print("Number of different screenshots:", n_different)
    if n_different < 4:
        sys.exit("The computer players did not make moves")
    page.close()


with sync_playwright() as p:
    browser = p.chromium.launch(channel="chrome")
    errors = []
    test_clicks(browser, errors)
    test_computer_players(browser, errors)
    browser.close()

if errors:
    sys.exit(f"Page errors: {errors}")
print("All browser tests passed")
