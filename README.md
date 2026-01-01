Built for Linux, requires `lm-sensors`.

Just build the two C programs, replacing `/path/to/buffer/`, and place them somewhere that is referenced in your $PATH, then ensure the following CRON job is in your crontab (`crontab -e`):

```
* * * * * /path/to/gettemp.sh
```

After a few minutes, run `thermal` to see the histogram. You might need to alter `gettemp.sh`'s `sensors` read depending on your hardware.
