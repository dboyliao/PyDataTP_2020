# Requirements

- [`mbed-cli`](https://os.mbed.com/docs/mbed-os/v6.4/build-tools/install-and-set-up.html): build tool for `mbed`
- [`utensor_cgen`](https://github.com/uTensor/utensor_cgen)

If you use [`pipenv`](https://github.com/pypa/pipenv), you can setup the environment by running `pipenv install`

# Compilation

## Gather Libraries

```bash
$ mbed deploy
```

This will clone `mbed-os` and `uTensor` runtime to the directory.

Should need to run only **once**.

## Generate Model Files

```bash
$ utensor-cli convert simple_cnn.tflite
```

## Compile and Flush

Note: You have to attach your dev board to your computer first.

```bash
# with dev board attached, run this command
$ mbed compile -m auto -t GCC_ARM -f --sterm
```