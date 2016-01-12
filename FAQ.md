# FAQ #

## Q. What is lol2dae? ##
**A.** lol2dae is an open source application written in C++ that converts models from the game League of Legends to the COLLADA format.

## Q. Why do I need lol2dae to convert League of Legends models? ##
**A.** League of Legends models are stored in a proprietary format with a .skn filename extension which is not natively supported by any 3D applications.

## Q. Do I need any 3rd party applications to run lol2dae? ##
**A.** No, it’s a standalone application.

## Q. What is COLLADA? ##
**A.** COLLADA (from collaborative design activity) is an interchange file format for interactive 3D applications. COLLADA documents describe digital assets in XML files, usually identified with a .dae (digital asset exchange) filename extension.

## Q. Does my 3D application of choice support the COLLADA format? ##
**A.** Most likely yes, COLLADA is a publicly available and ubiquitous format supported by many 3D applications.

## Q. I can't find any League of Legends models on my computer, where are they located? ##
**A.** They are stored in Riot Archive Files (RAF) with a .raf filename extension. You require an application like [RAFExtractor](https://sourceforge.net/projects/rafextractor/) with the capability to extract models with a.skn filename extension from these archives located in your League of Legends installation folder before you can convert them with lol2dae.

## Q. What platforms are supported? ##
**A.** Right now only Windows is supported.

## Q. My model has no texture, how do I fix it? ##
**A.** Textures are stored alongside the .skn file in the DirectDraw Surface (DDS) format. You can extract textures using an application like [RAFExtractor](https://sourceforge.net/projects/rafextractor/). Some 3D applications such as Maya natively support the DDS format, others will require a plug-in or conversion to another format.

To convert, NVIDIA provides [NVIDIA Texture Tools for Adobe Photoshop](https://developer.nvidia.com/nvidia-texture-tools-adobe-photoshop) for free to open DDS files in Photoshop. There are many applications and picture viewers (eg. [XnView](http://www.xnview.com)) that can convert DDS files also.

## Q. Is rigging or animation conversion supported? ##
**A.** Yes, lol2dae currently contains the functionality to output fully rigged and weighted models. Applying animations to a model's skeleton is also supported.

## Q. Will lol2dae convert newer models? ##
**A.** As of February 2015 all file versions are supported, but please report any bugs or issues with specific models.