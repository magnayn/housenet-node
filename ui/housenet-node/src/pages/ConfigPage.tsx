import { Button, Table, TableBody, TableCell, TableHead, TableRow, TextField } from "@mui/material";
import { useEffect, useState } from "react";
import Configuration  from '../Config';

// {"ip":"192.168.2.103",
// "version":"1.2.0",
// "build":"Mar 13 2022 14:00:18","sha1":"afec3753fec0d4dc76ddd02dccae08d57e265451","free":242320,"processing":true,"uptime_mins":429499}

export default function ConfigPage() {
    
    const [configuration, setConfiguration] = useState<string>("");
  
    function validJson() {
      try {
        JSON.parse(configuration);
        return true;
      } catch(e) {
        return false;
      }
    }

    useEffect( () => {

        fetch(`${Configuration.baseUrl}/housenet/config`)           
            .then(res => res.text())
            .then(data => setConfiguration(data));
        

    }, []);

    function saveConfig() {
      fetch(`${Configuration.baseUrl}/housenet/config`, {
        method: 'POST', body: configuration
      });
    }

    function reset() {
      fetch(`${Configuration.baseUrl}/reset`, {
        method: 'POST'
      });
    }

    return (
     <div>
       <TextField
          fullWidth={true}
          label="Configuration"
          multiline
          rows={25}
          value={configuration}
          onChange={(e)=>setConfiguration(e.target.value)}
        />
        <br/>
         <Button disabled={!validJson()} variant="contained" onClick={()=>saveConfig()}>Save Config</Button>
         <Button variant="contained" onClick={()=>reset()}>Reset</Button>  
      </div>
        
    );
}
