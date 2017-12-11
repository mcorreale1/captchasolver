from claptcha import Claptcha

# Initialize Claptcha object with "Text" as text and FreeMono as font
c = Claptcha("Text", "/Library/Fonts/AppleGothic.ttf")

# Get PIL Image object
text, image = c.image

print(text)        # 'Text'
print(type(image))  # <class 'PIL.Image.Image'>

# Get BytesIO object (note that it will represent a different image, just
# with the same text)
text, bytes = c.bytes

print(text)         # 'Text'
print(type(bytes))  # <class '_io.BytesIO'>

# Save a PNG file 'test.png'
text, file = c.write('test.png')

print(text)         # 'Text'
print(file)         # 'test.png'
