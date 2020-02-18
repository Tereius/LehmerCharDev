# LehmerCharDev

Provides a very fast pseudorandom number generator (named after D. H. Lehmer) as a kernel character device. This devices writes pseudorandom numbers at comparable speeds to /dev/zero and can be used for I/O filesystem benchmarks. __Please make sure that you never use this device for cryptographic purposes!__

To give you an idea of the transfer rates I ran some tests with dd tool:

| dd bs=        | /dev/lehmer   | /dev/zero |
| ------------- | ------------- | --------- |
| 1             | 3.0 MB/s      | 3.8 MB/s  |
| 512           | 1.5 GB/s      | 1.8 GB/s  |
| 4k            | 4.9 GB/s      | 10.4 GB/s |

## How to install

Besides the essential build infrastructure (gcc, make, binutils) you need CMake and the kernel headers of your current kernel.

To build the kernel module just run:

```
cmake ./ -G "Unix Makefiles" -B ./build_dir
cmake --build ./build_dir
```

To install the kernel module and add the character device `/dev/lehmer`:

```
cmake --build ./build_dir --target install
```

If the install command fails with _"Required key not available"_ you probably have to __disable Secure Boot__ in UEFI settings.


## How to use

You should be abel to use the device like so:

```
cat /dev/lehmer
```

To measure the transfer rates run:

```
dd if=/dev/lehmer of=/dev/null bs=512 count=10000
```

## How to uninstall

To uninstall the kernel module and character device:

```
cmake --build ./build_dir --target uninstall
```
