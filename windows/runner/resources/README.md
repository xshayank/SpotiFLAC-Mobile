# Application Icon

To use a custom icon for the Windows application:

1. Convert your icon.png to an .ico file format with multiple sizes (16x16, 32x32, 48x48, 256x256)
2. Place the resulting file as `app_icon.ico` in this directory
3. Rebuild the application

You can use online tools like:
- https://convertio.co/png-ico/
- https://www.icoconverter.com/

Or command-line tools like ImageMagick:
```
convert icon.png -define icon:auto-resize=256,128,64,48,32,16 app_icon.ico
```
