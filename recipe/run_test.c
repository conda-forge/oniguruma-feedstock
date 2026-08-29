#include <stdio.h>
#include <string.h>

#include <oniguruma.h>

int main(void) {
  static const UChar pattern[] = "jq";
  static const UChar text[] = "native jq";
  OnigEncoding encodings[] = {ONIG_ENCODING_UTF8};
  OnigErrorInfo error_info;
  OnigRegion* region;
  OnigRegex regex;
  int result;

  result = onig_initialize(encodings, 1);
  if (result != ONIG_NORMAL) {
    return 1;
  }

  result = onig_new(&regex,
                    pattern,
                    pattern + strlen((const char*)pattern),
                    ONIG_OPTION_DEFAULT,
                    ONIG_ENCODING_UTF8,
                    ONIG_SYNTAX_DEFAULT,
                    &error_info);
  if (result != ONIG_NORMAL) {
    onig_end();
    return 1;
  }

  region = onig_region_new();
  if (region == NULL) {
    onig_free(regex);
    onig_end();
    return 1;
  }

  result = onig_search(regex,
                       text,
                       text + strlen((const char*)text),
                       text,
                       text + strlen((const char*)text),
                       region,
                       ONIG_OPTION_NONE);

  if (result != 7 || region->num_regs < 1 || region->beg[0] != 7 ||
      region->end[0] != 9) {
    onig_region_free(region, 1);
    onig_free(regex);
    onig_end();
    return 1;
  }

  onig_region_free(region, 1);
  onig_free(regex);
  onig_end();
  puts("Oniguruma native API smoke test passed.");
  return 0;
}
