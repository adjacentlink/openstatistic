/*
 * Copyright (c) 2013,2016 - Adjacent Link LLC, Bridgewater,
 * New Jersey
 * Copyright (c) 2010 - DRS CenGen, LLC, Columbia, Maryland
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
 * * Neither the name of DRS CenGen, LLC nor the names of its
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

template<typename T>
OpenStatistic::StatisticNumeric<T>::StatisticNumeric():
  value_{}{}

template<typename T>
OpenStatistic::StatisticNumeric<T>::~StatisticNumeric(){}

template<typename T>
OpenStatistic::StatisticNumeric<T> & OpenStatistic::StatisticNumeric<T>::operator++()
{
  std::lock_guard<std::mutex> m(mutex_);

  ++value_;

  return *this;
}

template<typename T>
const T OpenStatistic::StatisticNumeric<T>::operator++(int)
{
  std::lock_guard<std::mutex> m(mutex_);

  T tmp{value_};

  ++value_;

  return tmp;
}

template<typename T>
OpenStatistic::StatisticNumeric<T> & OpenStatistic::StatisticNumeric<T>::operator+=(const OpenStatistic::StatisticNumeric<T> & rhs)
{
  std::lock_guard<std::mutex> m(mutex_);

  value_ += rhs.value_;

  return *this;
}

template<typename T>
const T OpenStatistic::StatisticNumeric<T>::operator+(const OpenStatistic::StatisticNumeric<T> & rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  T tmp{value_};

  tmp += rhs;

  return tmp;
}

template<typename T>
OpenStatistic::StatisticNumeric<T> & OpenStatistic::StatisticNumeric<T>::operator--()
{
  std::lock_guard<std::mutex> m(mutex_);

  --value_;

  return *this;
}

template<typename T>
const T OpenStatistic::StatisticNumeric<T>::operator--(int)
{
  std::lock_guard<std::mutex> m(mutex_);

  T tmp{value_};

  --value_;

  return tmp;
}

template<typename T>
const T OpenStatistic::StatisticNumeric<T>::operator-(const OpenStatistic::StatisticNumeric<T> & rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  T tmp{value_};

  tmp -= rhs;

  return tmp;
}

template<typename T>
OpenStatistic::StatisticNumeric<T> & OpenStatistic::StatisticNumeric<T>::operator-=(const OpenStatistic::StatisticNumeric<T> & rhs)
{
  std::lock_guard<std::mutex> m(mutex_);

  value_ -= rhs.value_;

  return *this;
}

template<typename T>
const T OpenStatistic::StatisticNumeric<T>::operator*(const OpenStatistic::StatisticNumeric<T> & rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  T tmp{value_};

  tmp *= rhs;

  return tmp;
}

template<typename T>
OpenStatistic::StatisticNumeric<T> & OpenStatistic::StatisticNumeric<T>::operator*=(const OpenStatistic::StatisticNumeric<T> & rhs)
{
  std::lock_guard<std::mutex> m(mutex_);

  value_ *= rhs.value_;

  return *this;
}

template<typename T>
const T OpenStatistic::StatisticNumeric<T>::operator/(const OpenStatistic::StatisticNumeric<T> & rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  T tmp{value_};

  tmp /= rhs;

  return tmp;
}

template<typename T>
OpenStatistic::StatisticNumeric<T> & OpenStatistic::StatisticNumeric<T>::operator/=(const OpenStatistic::StatisticNumeric<T> & rhs)
{
  std::lock_guard<std::mutex> m(mutex_);

  value_ /= rhs.value_;

  return *this;
}

template<typename T>
OpenStatistic::StatisticNumeric<T> & OpenStatistic::StatisticNumeric<T>::operator=(const OpenStatistic::StatisticNumeric<T> & rhs)
{
  std::lock_guard<std::mutex> m(mutex_);

  value_ = rhs.value_;

  return *this;
}

template<typename T>
OpenStatistic::StatisticNumeric<T> & OpenStatistic::StatisticNumeric<T>::operator+=(const T & rhs)
{
  std::lock_guard<std::mutex> m(mutex_);

  value_ += rhs;

  return *this;
}

template<typename T>
T OpenStatistic::StatisticNumeric<T>::operator+(const T & rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ + rhs;
}


template<typename T>
T OpenStatistic::StatisticNumeric<T>::operator-(const T & rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ - rhs;
}

template<typename T>
OpenStatistic::StatisticNumeric<T> & OpenStatistic::StatisticNumeric<T>::operator-=(const T & rhs)
{
  std::lock_guard<std::mutex> m(mutex_);

  value_-= rhs;

  return *this;
}

template<typename T>
T OpenStatistic::StatisticNumeric<T>::operator*(const T & rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ * rhs;
}

template<typename T>
OpenStatistic::StatisticNumeric<T> & OpenStatistic::StatisticNumeric<T>::operator*=(const T & rhs)
{
  std::lock_guard<std::mutex> m(mutex_);

  value_ *= rhs;

  return *this;
}

template<typename T>
T  OpenStatistic::StatisticNumeric<T>::operator/(const T & rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ / rhs;
}

template<typename T>
OpenStatistic::StatisticNumeric<T> & OpenStatistic::StatisticNumeric<T>::operator/=(const T & rhs)
{
  std::lock_guard<std::mutex> m(mutex_);

  value_ /= rhs;

  return *this;
}


template<typename T>
OpenStatistic::StatisticNumeric<T> & OpenStatistic::StatisticNumeric<T>::operator=(const T & rhs)
{
  std::lock_guard<std::mutex> m(mutex_);

  value_ = rhs;

  return *this;
}


template<typename T>
bool OpenStatistic::StatisticNumeric<T>::operator==(const OpenStatistic::StatisticNumeric<T> &rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ == rhs.value_;
}

template<typename T>
bool OpenStatistic::StatisticNumeric<T>::operator!=(const OpenStatistic::StatisticNumeric<T> &rhs) const
{
  return !operator==(rhs);
}

template<typename T>
bool OpenStatistic::StatisticNumeric<T>::operator<(const OpenStatistic::StatisticNumeric<T> &rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ < rhs.value_;
}

template<typename T>
bool OpenStatistic::StatisticNumeric<T>::operator<=(const OpenStatistic::StatisticNumeric<T> &rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ <= rhs.value_;
}

template<typename T>
bool OpenStatistic::StatisticNumeric<T>::operator>(const OpenStatistic::StatisticNumeric<T> &rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ > rhs.value_;
}

template<typename T>
bool OpenStatistic::StatisticNumeric<T>::operator>=(const OpenStatistic::StatisticNumeric<T> &rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ >= rhs.value_;
}

template<typename T>
bool OpenStatistic::StatisticNumeric<T>::operator==(const T & rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ == rhs;
}

template<typename T>
bool OpenStatistic::StatisticNumeric<T>::operator!=(const T & rhs) const
{
  return value_ != rhs;
}

template<typename T>
bool OpenStatistic::StatisticNumeric<T>::operator<(const T & rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ < rhs;
}

template<typename T>
bool OpenStatistic::StatisticNumeric<T>::operator<=(const T & rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ <= rhs;
}

template<typename T>
bool OpenStatistic::StatisticNumeric<T>::operator>(const T & rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ > rhs;
}

template<typename T>
bool OpenStatistic::StatisticNumeric<T>::operator>=(const T & rhs) const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_ >= rhs;
}

template<typename T>
T OpenStatistic::StatisticNumeric<T>::get() const
{
  std::lock_guard<std::mutex> m(mutex_);

  return value_;
}

template<typename T>
OpenStatistic::Any OpenStatistic::StatisticNumeric<T>::asAny() const
{
  std::lock_guard<std::mutex> m(mutex_);

  return Any{value_};
}

template<typename T>
void OpenStatistic::StatisticNumeric<T>::clear()
{
  std::lock_guard<std::mutex> m(mutex_);

  value_ = 0;
}

template<typename T>
template<typename Compare>
bool OpenStatistic::StatisticNumeric<T>::compareExchange(const T & expected, const T & desired)
{
  Compare cmp{};

  std::lock_guard<std::mutex> m(mutex_);

  if(cmp(value_,expected))
    {
      value_ = desired;

      return true;
    }

  return false;
}

template<typename T>
template<typename Compare>
bool OpenStatistic::StatisticNumeric<T>::compareExchange(const T & expected,
                                                         const T & desired,
                                                         const T & alternate)
{
  Compare cmp{};

  std::lock_guard<std::mutex> m(mutex_);

  if(cmp(value_,expected))
    {
      value_ = desired;

      return true;
    }
  else
    {
      value_ = alternate;
    }

  return false;
}

template <typename T>
bool operator<=(const T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return OpenStatistic::StatisticNumeric<T>(val)<=stat;
}

template <typename T>
bool operator>=(const T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return OpenStatistic::StatisticNumeric<T>(val)>=stat;
}

template <typename T>
bool operator>(const T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return val > stat.get();
}

template <typename T>
bool operator<(const T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return val > stat.getValue();
}

template <typename T>
bool operator==(const T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return val == stat.getValue();
}

template <typename T>
bool operator!=(const T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return val != stat.getValue();
}

template <typename T>
T operator+(const T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return val + stat.getValue();
}

template <typename T>
T operator-(const T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return val - stat.getValue();
}

template <typename T>
T operator*(const T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return val * stat.getValue();
}

template <typename T>
T operator/(const T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return val / stat.getValue();
}

template <typename T>
T & operator+=(T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return val += stat.getValue();
}

template <typename T>
T & operator-=(T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return val -= stat.getValue();
}

template <typename T>
T & operator*=(T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return val *= stat.getValue();
}

template <typename T>
T & operator/=(T & val, const OpenStatistic::StatisticNumeric<T> & stat)
{
  return val /= stat.getValue();
}
