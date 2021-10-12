=========
autopitch
=========

Basic Information
=================

**Autopitch** is a tool to tune your pitch in real time.

The GitHub repository : https://github.com/chilin0525/autopitch

Problem to Solve
================

I had terrible experience of singing test in music class when I in senior high school. The teacher ask me go back to the sit and spend more time practicing after I just song first lyric of song. But the problem is that I cannot understand how bad of my performance since I cannot recognize the pitch. So I want to build a tool to tune pitch in real time. There are a software call **autotune** also can tune your pitch, but it need lots of experience and tune by manually.  This is why the project named **autopitch** since we can use the tool built by ourselves to tune the pitch and save next student cannot recognize the pitch! 

The step to solve the problem:

1. Receive and parse audio, we should understand how computer store these audio signal at first.
2. We need recognizing the pitch, In audio signal processing call **pitch tracking algorithm** or **pitch detection algorithm**.
3. Do pitch shift to tune your wrong pitch.
4. Output correct pitch.

There are two types of pitch tracking algorithm:

1. Time-domain method: In time-domain method, **auto-correlation function(ACF)** is a classical method to do pitch tracking. Choosing the sample signal at first, and shift it from the sample signal we will get another signal, compare how similar between each other. If there are pretty similar, we can get the fundamental frequency. The disadvantage of time-domain method is that it **only suitable for monophonic signal** (just one voice).
2. Frequency-domain methods: Convert the audio signal to frequency spectrum by FFT at first, and attempts to find fundamental frequency by the frequency spectrum. Frequency-domain methods is better than Time-domain method since **it can detect polyphonic audio signal**. But it also **need more computation** in process, which may be harmful to this project since we need doing pitch tracking in real time.

(In this project, I will implement two method simultaneously(**Yin algorithm[1]** for time-domain method, **Harmonic product spectrum[2]** for frequency-domain method) since I cannot predict which one is more suitable for the project.)

TBD: pitch shift

Prospective Users
=================

1. Autopitch is useful for those people who want to tune their pitch in real time.
2. Autopitch provide pitch tracking algorithm such as Yin algorithm and Harmonic product spectrum in C++, and binding to Python. It is benefit for those people who also need pitch tracking algorithm.

System Architecture
===================
TBD: the following is a draft architecture

.. image:: https://i.imgur.com/dFNHcDN.png
  :width: 300px

API Description
===============
TBD

Engineering Infrastructure
==========================

* build system
    * make
* testing framework
    * C++: TBD
    * Python: Pytest
* version control: Git & Github
    * TBD: all branch name and function description

Schedule
========

* 10/10-10/16: 

  1. Study basic audio signal processing, YIN algorithm, Harmonic product spectrum. (since I have no experience of audio signal processing)
  2. update proposal.

* 10/17-10/23: 

  1. study basic audio signal processing and algorithm. Update proposal.
  2. update proposal.

* 10/24-10/30: 

  1. Implement parser to parse audio file.

* 10/31-11/06: 

  1. Implement Yin algorithm.

* 11/07-11/13: 

  1. Implement Harmonic product spectrum.

* 11/14-11/20: 

  1. Integrate parser with Yin algorithm and Harmonic product spectrum.
  2. Write testing to check whether result is correct or not.

* 11/21-11/27: 

  1. Implement pitch shifting.

* 11/28-12/04: 

  1. Integrate with pitch shifting.
  2. Test whole project.

* 12/05-12/11: 

  1. Extend the parser can parse real time audio signal from audio input device such as microphone.

* 12/12-12/18: 

  1. Extend whole project to accept input audio and do pitch shift in real time.

* 12/19-12/25: Presentation prepare.



References
-----------

1. [YIN, a fundamental frequency estimator for speech and music]:http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf
2. [Harmonic product spectrum]: rnnoildxnd re