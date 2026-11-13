# Bible-CLI
Bible-cli is a CLI program for reading the Bible in your terminal. It's written in C++. Currently, it supports only Windows, and there's only the WEBC version of the Bible.

# Instalation
To install Bible-CLI, download the [bible-cli-installer](//////url). When installed, type in your terminal commands for Bible-CLI.

# Basic usage
To use the program, first type either `bible` or `bible-cli` and then the options and arguments.

Using the `-p` or `--passage` options, you can put a passage you want to read. The program uses the same chapter-and-verse referencing system as the Bible, following the format:
- **Single verse:** `book_name chapter:verse` (e.g., `"Genesis" 1:1`)
- **Verse range:** `book_name chapter:from_verse-to_verse` (e.g., `"Genesis" 1:1-5`)

References identify the book, chapter, and specific verse or range of verses.
If you put a book's name that has one or more spaces, make sure to put it in quotation marks (e.g., "2 Maccabees").

# Options
These are the available options for Bible-CLI:
```
-h, --help                           Shows this help message
-v, --version                        Shows the version of this program
-p, --passage                        Set the Bible passage you want to get
-nl, --new-line                      Makes every verse be in their own line
-pnl, --passage-new-line             Makes the Bible passage be in their own line
-q, --quote                          Adds quotation makrs at the begining and end of the output (doesn't include the passage)
```

# Changing Bible Versions
Currently, there's only one version of the Bible that comes default with installing bible-cli. In the future, Bible-CLI will have a wide range of Bible versions to choose from.

# FAQ
## Q: Can I write the whole passage in one string?
No, the program won't work because of the way it reads the passage. You can put a book's name in one string and the chapter or chapter with verse/verses in another string.
## Q: Can I write the book without quotes?
With book names without spaces, yes. With book names with spaces, no. It's good practice to always use quotes for book names because of names with spaces. E.g., `Genesis` will work, but `2 Maccabees` won't.
## Q: Can I write the chapter and verse/verses separately?
No, the chapter and verse/verses have to be together or the program won't work.
## Q: I forgot a command/commands. What should I do?
Run `bible -h`, `bible --help`, `bible-cli -h` or `bible-cli --help` to view the options.

# Contributing
For now, there are no contributors needed.

# Final message
God bless you!
