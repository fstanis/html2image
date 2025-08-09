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
curl -o /tmp/sdk.7z -L 'https://ultralight-sdk-dev.sfo2.cdn.digitaloceanspaces.com/ultralight-sdk-min-latest-linux-x64.7z'
7z x /tmp/sdk.7z -o./ultralight-sdk
export ULTRALIGHT_SDK_DIR="$(realpath ./ultralight-sdk)"
```
https://ul-sdk.nyc3.digitaloceanspaces.com/1.4.0/linux/x64/release/ultralight-free-sdk-1.4.0-linux-x64.7z?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=DO801ZCVN6ZXPWYZ4HTE%2F20250809%2Fnyc3%2Fs3%2Faws4_request&X-Amz-Date=20250809T151335Z&X-Amz-Expires=300&X-Amz-Signature=24f685c5ab0384d3f62fb5a41cddd7e28483027a08025fa41bbfd26de1c910af&X-Amz-SignedHeaders=host&x-amz-checksum-mode=ENABLED&x-id=GetObject
### Building

If you have `github.com/fstanis/html2image` as a dependency, you must provide
the path to the Ultralight SDK downloaded in the previous steps via the
`CGO_CXXFLAGS` and `CGO_LDFLAGS` environment variables.

This examples assumes you set `ULTRALIGHT_SDK_DIR` in the previous step.

```bash
cd /path/to/your/project
go mod tidy

CGO_CXXFLAGS="-I$ULTRALIGHT_SDK_DIR/include" CGO_LDFLAGS="-L$ULTRALIGHT_SDK_DIR/bin" go build .
```

### Required runtime files

The following files are required at runtime:

-   cacert.pem
-   icudt67l.dat
-   libAppCore.so
-   libUltralightCore.so
-   libUltralight.so
-   libWebCore.so

These files should be in the subdirectory named `ultralight` relatitve to your
binary. They can be found in the Ultralight SDK.

The following command copies them from their respective locations.

```bash
cd /path/to/your/project
mkdir ultralight

cp \
  "$ULTRALIGHT_SDK_DIR/bin/libAppCore.so" \
  "$ULTRALIGHT_SDK_DIR/bin/libUltralightCore.so" \
  "$ULTRALIGHT_SDK_DIR/bin/libUltralight.so" \
  "$ULTRALIGHT_SDK_DIR/bin/libWebCore.so" \
  "$ULTRALIGHT_SDK_DIR/resources/cacert.pem" \
  "$ULTRALIGHT_SDK_DIR/resources/icudt67l.dat" \
  "./ultralight"
```

## Usage

Simple example that renders HTML into a PNG file.

```go
// create a new instance of a renderer with a viewport of 800x600 and scale 2.0
renderer := html2image.NewRenderer(800, 600, 2)
defer renderer.Free()

// render an image
img := renderer.Render("<div style='background-color:red'>Hello world</div>")

// write PNG file
f, _ := os.Create("image.png")
png.Encode(f, img)
f.Close()
```
