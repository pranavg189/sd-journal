/* Query log messages from the journal according to a field value in the current boot */

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

  /* Go to the end of the journal */
  r = sd_journal_seek_tail (j);
  if (r < 0)
  {
      fprintf(stderr, "Error seeking to the end of the journal: %s", strerror(-r));
  }

  /*  point to the latest log message */
  r = sd_journal_previous (j);
  if (r < 0)
  {
      fprintf(stderr, "Error retreating the read pointer in the journal: %s", strerror(-r));

  }


  /* Get the boot ID of the last entry in the journal */ 
  char *id;
  const char *latest_boot_id;
  size_t length;
  sd_journal_get_data (j , "_BOOT_ID", (const void **)&latest_boot_id, &length);

  printf("%.*s\n",(int) length, latest_boot_id);

  /* Add criteria for searching in the journal */
  sd_journal_add_match (j, "_COMM=sddm", 0);
  sd_journal_add_conjunction(j);
  sd_journal_add_match (j, latest_boot_id, 0);

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