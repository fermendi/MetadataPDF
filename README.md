-------------------------------------------------------------
# MetadataPDF
Simple program to change metadata and merge PDF files

Fernando Mendiburu - 2020
-------------------------------------------------------------

<p align="center">
  <img src="./resources/logo/logo.png" alt="Size Limit CLI" width="300">
</p>

# Table of Contents

- [Installation](#installation)
- [Dependences](#Dependences)
- [User Guide](#User-Guide)

## Installation

Go to `/home/user` directory:

```
$ cd ~
```

Download from [here](https://github.com/fermendi/MetadataPDF/archive/master.zip) or using git clone:

```
$ git clone https://github.com/fermendi/MetadataPDF.git
```

Create the build folder:

```
$ cd ~/MetadataPDF
$ mkdir build
$ cd build
```

For Linux, for example compile using qmake and the following commands:

```
$ qmake ~/MetadataPDF/MetadataPDF.pro -spec linux-g++ && /usr/bin/make qmake_all
$ make
```

## Dependences

`Qt`

`Ghostscript`

#### Ghostscript tips

See the tips in this [page](http://milan.kupcevic.net/ghostscript-ps-pdf/).

#### Frameless window

MetadataPDF project uses a modification of a MainWindow class with frameless window,
for more information about the project click [here](https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle).

## User Guide

`MetadataPDF` GUI for change Metadata is shown below:

<p align="center">
  <img src="./resources/gui/GUI_Metadata.png" alt="Size Limit CLI" width="600">
</p>


1. Press the button `"Select PDF"`.

2. Select the PDF that you want to change the metadata (PDF name and path file without spaces).

3. Change the fields of the metadata file as desired (title, author, etc).

4. Press the button `"Convert Metadata"`.

5. Find the output file with the desired metadata in the same path of the original PDF file.


`MetadataPDF` GUI for merge PDF files is shown below:

<p align="center">
  <img src="./resources/gui/GUI_Merge.png" alt="Size Limit CLI" width="600">
</p>


1. Add PDF files using button `"Add PDF"`.

2. Remove files added incorrectly (`"Remove last PDF"` button).

3. Press the button `"Merge PDFs"`, merged file is saved as `"MetadataPDF_Merged.pdf"`.
