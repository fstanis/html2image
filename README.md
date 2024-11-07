# html2image

`html2image` is a simple Go package that renders the given HTML string into an
[`Image`](https://pkg.go.dev/image#Image).

It uses [Ultralight](https://ultralig.ht/) under the hood to achieve this.

## How to use

Note: Ultralight isn't distributed with this library. Please make sure you're
familiar with the limitations of their [Free License](https://ultralig.ht/pricing/)
before you proceed.

### Dependencies

See [Ultralight's prerequisites](https://docs.ultralig.ht/docs/installing-prerequisites)
for more information. If you're on Ubuntu, the following should be enough to get
you started.

```bash
apt-get install \
  build-essential \
  clang \
  clang-format \
  clang-tidy \
  clang-tools \
  clangd \
  cmake \
  libc++-dev \
  libc++1 \
  libc++abi-dev \
  libc++abi1 \
  libclang-dev \
  libclang1 \
  libglu1-mesa-dev \
  liblldb-dev \
  libllvm-ocaml-dev \
  libomp-dev \
  libomp5 \
  libstdc++-12-dev \
  libx11-dev \
  lld \
  lldb \
  llvm \
  llvm-dev \
  llvm-runtime \
  python3-clang \
  xorg-dev
```

### Downloading the SDK

This library expects the Ultralight SDK to be in the `ultralight-sdk` folder in
this repo. You can [find the latest SDK here](https://ultralig.ht/download/).

Assuming you have `curl` and 7-zip installed, the following should work.

```bash
curl -o /tmp/sdk.7z -L 'https://ultralight-files.sfo3.cdn.digitaloceanspaces.com/ultralight-sdk-1.3.0-linux-x64.7z'
7z x /tmp/sdk.7z -o./ultralight-sdk
```

### `embeddedfiles.cpp`

Ultralight requires `resources/icudt67l.dat` and `resources/cacert.pem` to exist
in the filesystem in order to operate. `html2image` embeds these files and loads
them in-memory. These files are expected to be defined in
`wrapper/embeddedfiles.cpp` which is generated by the `embeddedfiles.sh` script.

```bash
sh embeddedfiles.sh
```

## Usage

Simple example that renders HTML into a PNG file.

```go
// create a new instance of a renderer with a viewport of 800x600
renderer := html2image.NewRenderer(800, 600, nil)
defer renderer.Free()

// render an image
img := renderer.Render("<div style='background-color:red'>Hello world</div>")

// write PNG file
f, _ := os.Create("image.png")
png.Encode(f, img)
f.Close()
```