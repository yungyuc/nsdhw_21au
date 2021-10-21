# Fourier Transform for Image Processing
## Basic Information
* GitHub repository: TODO
## Problem to Solve

The Fourier Transform is used in a wide range of image applications, which transform the signal from time domain to frequency domain, such as image analysis, image filtering, image reconstruction and image compression.

The formula of 2-D Discrete Fourier Transform for an M*N image:
> $\dfrac{1}{M*N}\sum_{x=0}^{M-1}\sum_{y=0}^{N-1}f(x,y)e^{-i2\pi[\dfrac{ux}{M}+\dfrac{vy}{N}]}$, for u = 0..M-1, v = 0..N-1
## Prospective Users
People who would like to do image processing tricks which are based on frequency domain.
## System Architecture
![](https://i.imgur.com/KtVQwGY.png)

## API Description
* Python
    * getParam(): parser
    * getResult()
* C++ 
    * FFT()
    * InverseFFT()
    * VagueProcess(): LowPassFilter
    * EdgeDetection(): HighPassFilter
    * HybridImage(): LowPassFilter+HighPassFilter
    
## Engineering Infrastructure
* Build System: Makefile
    * make
    * make test
    * make clean
* Version Control: Git
## Schedule
* Week 1: setup Python and c++ environment
* Week 2: implement **getParam()**
* Week 3: implement **FFT()** and **InverseFFT()**
* Week 4: implememt **VagueProcess()** and **EdgeDetection()**
* Week 5: implement **HybridImage()** and **getResult()**
* Week 6: Testing
* Week 7: Project presentation
* Week 8: Project presentation
## References
https://homepages.inf.ed.ac.uk/rbf/HIPR2/fourier.htm