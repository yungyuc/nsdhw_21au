# Backtesting framework for trading strategy

## Basic Information
In quant trading, backtesting is the general method to assess the viability of a given strategy. It uses historical data to simulate the trading strategy and then generates a quantitative report.

Github repository: TBD

## Problem to Solve
Numerous open-source backtesting systems such as [Backtrader](https://www.backtrader.com/), [QSTrader](https://github.com/mhallsmoore/qstrader) and [backtesting.py](https://kernc.github.io/backtesting.py/) have been proposed and well-maintained by developers. They offer a very user-friendly interface that allows users to focus on the implementation of the strategy itself, and the whole structure of them is mostly written in python. The project aims to realize a simple version of backtesting framework, with similar basic functions but faster performance. Considering the diversity of trading strategies and the scope of framework implementation, the project will mainly focus on specific strategy and try to optimize its low-level implementation.

## Perspective Users
Those who are interested in trading simulation engine for research or business purposes.

## System Architecture
![image info](./image/System_Architecure.png)


## API Description
| Name | Inputs  | Returns  | Description  |
| ---- | ------------ | ------------ | ------------ |
| Backtester  |  single_factor_name, time_interval, strategy_mode | trade_reports  | Construct trade reports based on specific single factor. Some user config (e.g. database path & benchmark) should be set first  |
| SingleFactorBuilder  |  predefined_factor_function  | factor_values_csv  | Construct single factor values using given function. |
| TestEngine  |  position, trad_limit ,benchmark_returns, strategy_info etc.  | daily_trade_result  | Construct single factor values using given function. |

## Engineer Infrastructure
- Build system: make
- Test framework:
  - Python: Pytest
  - C++: GoogleTest
- Version control: git & Github

## Schedule
| Week | Schedule  |
| ------------ | ------------ |
| 1  | study code & review domain knowledge  |
| 2  | implementation of basic framework  |
| 3  | unit test for `Backtester` & `SingleFactorBuilder` |
| 4  | implementation of `TestEngine`  |
| 5  | unit test for `TestEngine` |
| 6  | minor improvement & bug fixes  |
| 7  | preparing presentation |

