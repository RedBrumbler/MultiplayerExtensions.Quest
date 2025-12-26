# MultiplayerExtensions (Quest/Standalone-Only)
A Beat Saber mod which aims to improve the multiplayer experience.

## Features
* Fancy platform lighting
* Fancy in-game lighting
**Looking for more features, feel free to contribute your own.**

## Installation
1. Download the `MultiplayerExtensionsq.qmod` file listed under `Assets` **[Here](https://github.com/EnderdracheLP/MultiplayerExtensions.Quest/releases)**.
   * Optionally, you can get a development build by downloading the file listed under `Artifacts`  **[Here](https://github.com/EnderdracheLP/MultiplayerExtensions.Quest/actions)** (pick the topmost successful build).
   * You must be logged into GitHub to download a development build.
2. Upload the `MultiplayerExtensions.qmod` using your preferred modding tool, for example [MBF](mbf.bsquest.xyz).

Enjoy!

## Reporting Issues
* The best way to report issues is to click on the `Issues` tab at the top of the GitHub page. This allows any contributor to see the problem and attempt to fix it, and others with the same issue can contribute more information. **Please try the troubleshooting steps before reporting the issues listed there. Please only report issues after using the latest build, your problem may have already been fixed.**
* Include in your issue:
  * A detailed explanation of your problem (you can also attach videos/screenshots)
  * **Important**: The log file from the game session the issue occurred, you can use [MBF](mbf.bsquest.xyz) to get one
* If you ask for help on Discord, at least include your `logcat.log` file in your help request.

## Contributing
Anyone can feel free to contribute bug fixes or enhancements to MultiplayerExtensions. GitHub Actions for Pull Requests made from GitHub accounts that don't have direct access to the repository will fail. This is normal because the Action requires a `Secret` to download dependencies.
### Building
Visual Studio Code is recommended, ensure your environment is setup for quest modding see [here](https://bsmg.wiki/modding/quest/intro.html#environment-setup) for more information on setting up a dev environment.
1. Check out the repository
2. Open the project folder.
3. Run `qpm restore` to download all necessary dependencies.
4. Run `qpm s qmod`, the project should now build the mod and package it into a qmod.
### Testing
MultiplayerExtensions and other multiplayer mods may not work without a compatible private server to play on. It is designed for [BeatTogether](https://github.com/BeatTogether/BeatTogether), which comes in the form of it's [Master](https://github.com/pythonology/BeatTogether.MasterServer) and [Dedicated](https://github.com/pythonology/BeatTogether.DedicatedServer) servers. If you are looking to update this mod to a newer version, these servers will also need to be up to date and working for that version. You can gain access to their private beta by donating on their [patreon](https://www.patreon.com/BeatTogether). Alternatively, you can set up your own cluster.

## Donate
You can support development of MultiplayerExtensions by donating at the following links:
* https://github.com/sponsors/michael-r-elp
* https://www.patreon.com/goobwabber
* https://ko-fi.com/goobwabber
* https://ko-fi.com/zingabopp

## Related Mods
* [MultiplayerCore](https://github.com/EnderdracheLP/MultiplayerCore.Quest)
* BeatTogether for [PC](https://github.com/BeatTogether/BeatTogether) or [Quest](https://github.com/BeatTogether/BeatTogether.Quest)
* BeatSaberServerBrowser for [PC](https://github.com/roydejong/BeatSaberServerBrowser) or [Quest](https://github.com/EnderdracheLP/BeatSaberServerBrowserQuest)
