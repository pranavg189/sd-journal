/* Query log messages from the journal according to a field value */

#include <stdio.h>
#include <string.h>
#include <systemd/sd-journal.h>

int main(int argc, char *argv[]) {
  int r;
  sd_journal *j;

  r = sd_journal_open(&j, 0);

  if (r < 0) {
    fprintf(stderr, "Failed to open journal: %s\n", strerror(-r));
    return 1;
  }

  /* Add criteria for searching in the journal */
  sd_journal_add_match (j, "_COMM=sddm", 0);

  SD_JOURNAL_FOREACH(j) {
    const char *d;
    size_t l;

    r = sd_journal_get_data(j, "MESSAGE", (const void **)&d, &l);

    if (r < 0) {
      fprintf(stderr, "Failed to read message field: %s\n", strerror(-r));
      continue;
    }

    printf("%.*s\n", (int) l, d);
  }

  sd_journal_close(j);

  return 0;
}