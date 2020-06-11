#pragma once

#include <harfbuzz/hb.h>
#include <unicode/unistr.h>
#include <unicode/uscript.h>
#include <vector>

#include "context.hpp"
#include "msdf_font.hpp"

class MsdfFontAtlas {
public:
  void AddMsdfFont(const MsdfFont &font, const UScriptCode &icuScript,
                   const hb_script_t &hbScript);

  void SetDefault(const MsdfFont &font);

  std::vector<std::shared_ptr<MsdfGlyph>>
  CreateGlyphs(const Context &context, const icu::UnicodeString &str,
               const int &pixelSize);

private:
  void AddGlyphs(const Context &context,
                 std::vector<std::shared_ptr<MsdfGlyph>> &glyphs, float &x,
                 const UScriptCode &script, const icu::UnicodeString &str,
                 const int &pixelSize);

  struct Entry {
    MsdfFont font;
    UScriptCode icuScript;
  };

  std::vector<Entry> entries;
  MsdfFont default;
};