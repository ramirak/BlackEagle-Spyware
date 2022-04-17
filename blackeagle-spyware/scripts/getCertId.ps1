function Retrieve-ServerCertFromSocket ($hostname, $port=443, $SNIHeader, [switch]$FailWithoutTrust)
{
	$hostname=$hostname.split(':')[0]

    if (!$SNIHeader) {
        $SNIHeader = $hostname
    }

    $cert = $null
    try {
        $tcpclient = new-object System.Net.Sockets.tcpclient
        $tcpclient.Connect($hostname,$port)

        #Authenticate with SSL
        if (!$FailWithoutTrust) {
            $sslstream = new-object System.Net.Security.SslStream -ArgumentList $tcpclient.GetStream(),$false, {$true}
        } else {
            $sslstream = new-object System.Net.Security.SslStream -ArgumentList $tcpclient.GetStream(),$false
        }

        $sslstream.AuthenticateAsClient($SNIHeader)
        $cert =  [System.Security.Cryptography.X509Certificates.X509Certificate2]($sslstream.remotecertificate)

     } catch {
     } finally {
        #cleanup
        if ($sslStream) {$sslstream.close()}
        if ($tcpclient) {$tcpclient.close()}        
     }    
    return $cert
}
