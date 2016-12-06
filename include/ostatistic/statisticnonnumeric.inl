/*
 * Copyright (c) 2013,2016 - Adjacent Link LLC, Bridgewater, New Jersey
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of Adjacent Link LLC nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * See toplevel COPYING for more information.
 */

template <typename T>
OpenStatistic::StatisticNonNumeric<T>::StatisticNonNumeric():
  value_{}{}

template <typename T>
OpenStatistic::StatisticNonNumeric<T>::~StatisticNonNumeric(){}

template <typename T>
OpenStatistic::StatisticNonNumeric<T> &
OpenStatistic::StatisticNonNumeric<T>::operator=(const T & value)
{
  std::lock_guard<std::mutex> m(mutex_);
  value_ = value;
  return *this;
}

template <typename T>
T OpenStatistic::StatisticNonNumeric<T>::get() const
{
  std::lock_guard<std::mutex> m(mutex_);
  return value_;
}

template <typename T>
OpenStatistic::Any OpenStatistic::StatisticNonNumeric<T>::asAny() const
{
  std::lock_guard<std::mutex> m(mutex_);
  return Any{value_};
}

template <typename T>
void OpenStatistic::StatisticNonNumeric<T>::clear()
{
  std::lock_guard<std::mutex> m(mutex_);
  value_ = T{};
}
