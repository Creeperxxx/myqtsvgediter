@echo off
echo Report generated on %date% %time% >> codelines.txt
cloc .  >> codelines.txt