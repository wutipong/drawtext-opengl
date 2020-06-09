#pragma once

#include <harfbuzz/hb.h>
#include <unicode/unistr.h>
#include <unicode/uscript.h>
#include <vector>

#include "context.hpp"
#include "font.hpp"

class FontAtlas {
public:
  void AddFont(const Font &font, const UScriptCode &icuScript,
               const hb_script_t &hbScript);

  void SetDefault(const Font &font);

  std::vector<std::shared_ptr<Glyph>>
  CreateGlyphs(const Context &context, const icu::UnicodeString &str,
               const int &pixelSize);

private:
  void AddGlyphs(const Context &context,
                 std::vector<std::shared_ptr<Glyph>> &glyphs, float &x,
                 const UScriptCode &script, const icu::UnicodeString &str,
                 const int &pixelSize);

  struct Entry {
    Font font;
    UScriptCode icuScript;
  };

  std::vector<Entry> entries;
  Font default;
};