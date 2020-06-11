#include "msdf_font_atlas.hpp"

void MsdfFontAtlas::AddMsdfFont(const MsdfFont &font,
                                const UScriptCode &icuScript,
                                const hb_script_t &hbScript) {
  Entry e{font, icuScript};

  e.font.script = hbScript;
  entries.push_back(e);
}

void MsdfFontAtlas::SetDefault(const MsdfFont &font) { default = font; }

std::vector<std::shared_ptr<MsdfGlyph>>
MsdfFontAtlas::CreateGlyphs(const Context &context,
                            const icu::UnicodeString &str,
                            const int &pixelSize) {
  float x = 0;
  std::vector<std::shared_ptr<MsdfGlyph>> output;

  if (str.length() == 0) {
    return output;
  }
  UErrorCode error;
  auto script = uscript_getScript(str[0], &error);

  size_t start = 0, end;

  for (auto i = 0; i < str.length(); i++) {
    auto s = uscript_getScript(str[i], &error);
    if (s == script)
      continue;

    end = i;
    auto substr = str.tempSubString(start, end - start);
    AddGlyphs(context, output, x, script, substr, pixelSize);

    script = s;
    start = end;
  }

  auto substr = str.tempSubString(start);
  AddGlyphs(context, output, x, script, substr, pixelSize);

  return output;
}

void MsdfFontAtlas::AddGlyphs(const Context &context,
                              std::vector<std::shared_ptr<MsdfGlyph>> &glyphs,
                              float &x, const UScriptCode &script,
                              const icu::UnicodeString &str,
                              const int &pixelSize) {
  auto iter = std::find_if(entries.begin(), entries.end(),
                           [&script](auto e) { return e.icuScript == script; });

  MsdfFont *f = &default;
  if (iter != entries.end()) {
    f = &iter->font;
  }

  auto g = f->CreateGlyphs(context, x, str, pixelSize);
  glyphs.insert(glyphs.end(), g.begin(), g.end());
}