#pragma once

#include <tuple>

#include <Cabana_Core.hpp>

namespace storage {

template <typename... Types>
using CabanaDataTypes = Cabana::MemberTypes<Types...>;

template <typename... Types>
using CabanaTuple = Cabana::Tuple<CabanaDataTypes<Types...>>;

template <typename... Types> using Tuple = std::tuple<Types...>;

template <std::size_t Index, typename... Types> struct TupleParserBase {

  template <typename... FullTypes>
  static Tuple<> to_std(CabanaTuple<FullTypes...> &t) {
    return Tuple<>();
  }

  template <typename... FullTypes>
  static void to_cabana_base(const Tuple<FullTypes...> &source,
                             CabanaTuple<FullTypes...> &target) {}
};

template <std::size_t Index, typename T, typename... Types>
struct TupleParserBase<Index, T, Types...> {

  template <typename... FullTypes>
  static Tuple<T, Types...> to_std(CabanaTuple<FullTypes...> &t) {
    return std::tuple_cat(std::tie(Cabana::get<Index>(t)),
                          TupleParserBase<Index + 1, Types...>::to_std(t));
  }

  template <typename... FullTypes>
  static void to_cabana_base(const Tuple<FullTypes...> &source,
                             CabanaTuple<FullTypes...> &target) {
    Cabana::get<Index>(target) = std::get<Index>(source);
    TupleParserBase<Index + 1, Types...>::to_cabana_base(source, target);
  }
};

template <typename... Types>
struct TupleParser : public TupleParserBase<0, Types...> {
  static CabanaTuple<Types...> to_cabana(const Tuple<Types...> &source) {
    CabanaTuple<Types...> target;
    TupleParserBase<0, Types...>::to_cabana_base(source, target);
    return target;
  }
};

template <std::size_t Index, typename... Types> struct RefTupleExtractor {
  template <typename AosoaType>
  static Tuple<> get(AosoaType &data, std::size_t i) {
    return std::tie();
  }
};

template <std::size_t Index, typename T, typename... Types>
struct RefTupleExtractor<Index, T, Types...> {
  template <typename AosoaType>
  static Tuple<T &, Types &...> get(AosoaType &data, std::size_t i) {
    return std::tuple_cat(std::tie(Cabana::slice<Index, AosoaType>(data)(i)),
                          RefTupleExtractor<Index + 1, Types...>::get(data, i));
  }
};

template <std::size_t Index, typename... Types> struct RefTupleUpdator {
  template <typename... FullTypes>
  static void set(Tuple<FullTypes &...> &t, Types... ts) {}
};

template <std::size_t Index, typename T, typename... Types>
struct RefTupleUpdator<Index, T, Types...> {
  template <typename... FullTypes>
  static void set(Tuple<FullTypes &...> &t, T c, Types... ts) {
    std::get<Index>(t) = c;
    RefTupleUpdator<Index + 1, Types...>::set(t, ts...);
  }
};

template <std::size_t Index, typename T, typename... Types>
struct ExtractTypeAt {
  using Type = typename ExtractTypeAt<Index - 1, Types...>::Type;
};

template <typename T, typename... Types> struct ExtractTypeAt<0, T, Types...> {
  using Type = T;
};
} // namespace AosoaStorage