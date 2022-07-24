
# peeping-tom

###  Remote keylogger for Windows written in C++

Welcome to the peeping-tom project repo! Peeping-tom is a two-part program with a peeping-client deployed on a target and a toms-server listening for keylogs on the attacker machine. Getting up and running requires a decent amount of set up, so make sure to carefully read and follow the instructions below.

Of course, if you have any suggestions, feedback, issues, etc... feel free to reach out or create an issue or pull request. 

____

### Contents

- [Features](#features)
- [Pre-Requisites](#pre-requisites)
- [Instructions](#instructions)
- [Demo](#demo)
- [Troubleshooting](#troubleshooting)
- [How to protect yourself?](#how-to-protect-yourself)
- [Contributing](#contributing)
- [Disclaimer](#disclaimer)
- [Acknowledgement](#acknowledgement)
- [Contact](#contact)

____

### Features

- [x] Records ALL keystrokes (including system keys)
- [x] Compact size & low resource consumption
- [x] Customizable callback interval
- [x] Companion Python server
- [x] AES-256 encryption
- [x] Low AV detection

____

### Pre-Requisites

- [Visual Studio](https://visualstudio.microsoft.com/downloads/)  - The community edition is good enough for our purposes
- [Python](https://www.python.org/downloads/) - Download and install Python 3.9 for your OS
- [Ngrok](https://ngrok.com/download) - You will need a free Ngrok account as well as the Ngrok program to make your machine accessible from the internet

____

### Instructions

**Step 1: Open peeping-tom solutions file in Visual Studio**

First things first, download and install the community edition of Visual Studio onto your machine from the [link above](#pre-requisites). When moving through the installer, make sure to select the "Python development" and the "Desktop development with C++" workloads as they will be necessary to properly load and compile the project.

![Visual Studio Required Workloads](https://i.imgur.com/AfawekT.png)

Once everything has finished installing, launch Visual Studio and select the "Clone a repository" option. Enter the project's git URL and hit clone.

![Visual Studio Project Clone](https://i.imgur.com/LGDinMz.png)

If everything loaded correctly, you should see both projects in the solutions explorer window to the right as shown below.

![Visual Studio Solutions Explorer](https://i.imgur.com/FSZXhs1.png)

**Step 2: Configure compiler/linker options in Visual Studio**

Now that the tool is loaded into Visual Studio, we will need to change a couple of options and compile a library before building. This will remove any program/library dependencies when running on the target.

The first task is to statically compile the Crypto++ library which is used for encryption in the keylogger. Start this process by downloading version 8.6.0 of the library [here](https://www.cryptopp.com/#download).

Once you've downloaded the ZIP file, extract it to a folder, navigate into it, and launch the cryptest.sln with Visual Studio. You should now have two Visual Studio windows, one with the peeping-tom solution, and the other with cryptest solution open.

![Cryptest.sln File](https://i.imgur.com/Iwh9Bxd.png)

Once the cryptest.sln file has loaded into Visual Studio, click on "Debug" on the top toolbar and set it to "Release." Also change the architecture to x64 as shown below.

![Visual Studio Release Option](https://i.imgur.com/AmDDEEa.png)

Now, right-click on the cryptlib project, and select properties. Change the highlighted option as shown.

![Visual Studio Code Generation Option](https://i.imgur.com/XjbpjCP.png)

Once that is complete, build the cryptest solution.

![Visual Studio Build Project](https://i.imgur.com/H0gltNu.png)

The build will take a good minute or two, but once it's done, a file named cryptlib.lib should appear in the your version of the path below. Find the file and copy its path for the next step.

```console
C:\Users\Abdullah Beta\Downloads\cryptopp860\x64\Output\Release\cryptlib.lib
```

Once you've located the file and copied its path, return to the Visual Studio window that has the peeping-tom solution open. Right-click on the peeping-client project, and select properties. Navigate to the option showed in the screenshot below, and paste the path of cryptlib.lib.

![Visual Studio Linker Include](https://i.imgur.com/V6qjQ0f.png)

Make sure that the peeping-client project is also set to "Release" and that the architecture is set to x64. **DON'T** build the peeping-tom solution yet as there are still some changes that need to be made in the source code.

**Step 3: Open an public tunnel to localhost:80**

Onto to the tunnel. Once you've created an Ngrok account, download Ngrok for your OS from the [link above](#pre-requisites) and extract the executable from the ZIP file. Before opening a tunnel, you will need to grab a personal authentication token from your account as shown below.

![Ngrok Authtoken](https://i.imgur.com/fLOreRu.png)

Once you have the token, add it to Ngrok's local configuration with the following command:

```console
ngrok config add-authtoken 7cxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxmgZ5
```

Finally, go ahead and open the tunnel using the following command:

```console
ngrok http 80 --scheme=http
```

If all went well, you should see something similar to the image below. Great! Now you're all set for this step. Go ahead and grab the "Forwarding" address highlighted and save it someplace safe for later.

![Ngrok Tunnel URL](https://i.imgur.com/9PSgeVG.png)

**Step 4: Generate new AES-256 key**

At this point, you can go ahead and download Python onto your machine. Once that is installed, open a CMD window in the toms-server project directory.

Install the required dependencies with:

```console
pip install -r requirements.txt 
```

Once everything is ready, run:

```console
python3 toms-server.py -m keygen
```
This should output a 64 character AES-256 key which you will also need later on, so make sure to save it along with the Ngrok forwarding address.

**Step 5: Set required fields in peeping-client source**

Now, we have everything we need to begin. Open the *peeping-client project->source files->peeping-client.cpp* source file from the solutions explorer. Enter the relevant information in the first two variables of the main() function shown below.

![Option Setting](https://i.imgur.com/CcI8rty.png)

After your information is entered, head over to *peeping-client project->header files->APIHook.h* and replace the "15" in the image below with your desired interval for callback in **seconds** (i.e change 15 to 7200 for callback every two hours...you get the idea).

![Interval Setting](https://i.imgur.com/Rf6zz4K.png)

Finally, build the project and you should find an executable in your version of the following location:

```console
C:\Users\Abdullah Beta\source\repos\peeping-client\x64\Release
``` 

Congratulations! You now have a fully customized keylogger configured to callback with encrypted keyboard data at regular intervals.

**Step 6: Start listening server**

The final step is to begin listening for incoming keylogs from the target. You can start the server in listening mode with the following command:

```console
python3 toms-server.py -m server
```
Once the server has started, enter the exact AES key you generated earlier and used in the peeping-client.cpp source file. If the key is accepted, you should get a message saying that the server has been started!

**Step 7: Analyze logs + important notes**

All that's left is executing the newly-built peeping-client.exe on the target. I will the how of that to your creativity. Every time the interval you set passes, you will receive a message on the terminal. 

**Warning:**  ***Please be sure to not lose internet connection or close your Ngrok tunnel during this period as this would necessitate re-executing the peeping-client.exe program on the target.***

All keylogs will be written to a keylog,txt file in the directory of the toms-server script and can be viewed from there. That's all for the instructions, hope you were able to get up and running without any major hurdles!

____

### Demo

Coming soon!

____

### Troubleshooting

- If the toms-server project fails to load in Visual Studio becuase of insufficent permisssions, right-click on the project and select "Reload." This should rectify the issue.
- Even though I have added a requirements file for the python companion script, sometimes, the requests library used to generate AES keys does not install. In that case simply install it manually as shown below.

```console
pip install requests
```

____

### How to protect yourself?

1.  Use the on-screen keyboard for ultra-sensitive information
2.  Update your antivirus software regularly
3.  Check your firewall settings
4.	Install a secure password manager

____

### Contributing

When contributing to this repository, please discuss the changes you wish to make via issue, [email](mailto:abdullahansari1618@outlook.com), or [LinkedIn](https://www.linkedin.com/in/abdullahansari0/).

____

### Disclaimer

 This project is only for educational purposes. Any kind of bad behavior conducted with this project is the user's own responsibility. I hereby forfeit responsiblity for any illegal actions.
 
____

### Acknowledgement 

 This project was partially guided by  Ermin Kreponic's [Udemy course](https://www.udemy.com/course/how-to-create-an-advanced-keylogger-from-scratch-for-windows/?utm_source=adwords&utm_medium=udemyads&utm_campaign=DSA_Catchall_la.EN_cc.US&utm_content=deal4584&utm_term=_._ag_95911180068_._ad_532194018662_._kw__._de_c_._dm__._pl__._ti_dsa-437115340933_._li_9027793_._pd__._&matchtype=&gclid=Cj0KCQjwpv2TBhDoARIsALBnVnkvfomOaJmfcSnd_eFIFMl01QSMYnwYxlvFP-ig8LnZAamJqX28AywaAgivEALw_wcB) and was overhauled to incorporate some new ideas I had and new skills I had learned.

____

### Contact
Author - Abdullah Ansari ©

Contact Info - [Email](mailto:abdullahansari1618@outlook.com)

____