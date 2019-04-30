# Sliding-Sketch

## Introduction

Data stream processing in sliding windows is an important and challenging work. While existing works use one sketch to address one single query, we propose a generic framework in this paper, namely the Sliding sketch, which can be applied to most existing sketches and answer various kinds of queries in sliding windows. We use our framework to address three fundamental queries in sliding windows: membership query (the Bloom filter), frequency query (the CM sketch, the CU sketch, and the Count sketch) and heavy hitter query (HeavyKeeper). Experimental results show that after using our framework, the accuracy of existing sketches that do not support sliding windows becomes much higher than the algorithms for sliding windows. In membership query, the error rate of the Sliding sketch is 10-50 times lower than that of the state-of-the-art sliding window algorithm. In frequency query, ARE of the Sliding sketch is 40-50 times lower than that of the state-of-the-art sliding window algorithm. In heavy hitter query, the precision and recall of the Sliding sketch are near to 100% and better than the state-of-the-art, and ARE of the frequencies of heavy hitters in the Sliding sketch is 3 -5.6 times lower than that of the state-of-the-art sliding window algorithm.

## About this repo

This repo contains all the algorithms in our experiments, as shown in the following table.

| Task                      | Algorithms                               |
| ------------------------- | ---------------------------------------- |
| Membership query | Sl-BF, FBF, SW-BF |
| Frequency query   | Sl-CM,Sl-CU,Sl-Count,ECM,SWCM |
| HeavyHitter query   |    Sl-HK,lambda-Algorithm,WCSS  |

This repo also contains a small demo to show how to use this algorithms with a small dataset.

- `Sliding_sketch_technicual_report.pdf` can help you understand the paper better.
- `data`: traces for test, each 8 bytes in a trace is a item.
- `src`
  - `Membership`: Membership query algorithms (Sl-BF,FBF, SW-BF).
  - `Frequency`: Frequency query algorithms (Sl-CM,Cl-CU,Sl-Count,ECM,SWCM).
  - `HeavyHitter`: HeavyHitter query algorithm (Sl-HK,lambda-Algorithm,WCSS).
- `demo`: a small demo to show how to use these algorithms.

## Requirements
- cmake
- g++

We have  tested this demo on Ubuntu 14.04.5 LTS (GNU/Linux 3.16.0-50-generic x86_64).

## How to build
```
cd demo
/bin/sh demo.sh
```
