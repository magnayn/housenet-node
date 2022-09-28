import { Table, TableBody, TableCell, TableHead, TableRow } from "@mui/material";
import { useEffect, useState } from "react";
import Configuration  from '../Config';

// {"ip":"192.168.2.103",
// "version":"1.2.0",
// "build":"Mar 13 2022 14:00:18","sha1":"afec3753fec0d4dc76ddd02dccae08d57e265451","free":242320,"processing":true,"uptime_mins":429499}

export default function StatusPage() {
    
    const [statusInfo, setStatusInfo] = useState<any>({});
  
    useEffect( () => {

        fetch(`${Configuration.baseUrl}/housenet/status`)
            .then(res=> res.json())
            .then(data => setStatusInfo(data))
        

    }, []);

    return (
        <Table size="small">
        <TableHead>
          <TableRow>
            <TableCell>Item</TableCell>
            <TableCell>Value</TableCell>            
          </TableRow>
        </TableHead>
        <TableBody>         
            <TableRow >
              <TableCell>IP</TableCell>
              <TableCell>{statusInfo?.ip}</TableCell>
            </TableRow>          
            <TableRow >
              <TableCell>Version</TableCell>
              <TableCell>{statusInfo?.version}</TableCell>
            </TableRow>          
            <TableRow >
              <TableCell>Build</TableCell>
              <TableCell>{statusInfo?.build} <i>{statusInfo?.sha1}</i></TableCell>
            </TableRow>       
            <TableRow >
              <TableCell>Free (bytes)</TableCell>
              <TableCell>{statusInfo?.free}</TableCell>
            </TableRow>          
            <TableRow >
              <TableCell>Processing</TableCell>
              <TableCell>{statusInfo?.processing?"Yes":"No"}</TableCell>
            </TableRow>          
            <TableRow >
              <TableCell>Uptime (mins)</TableCell>
              <TableCell>{statusInfo?.uptime_mins}</TableCell>
            </TableRow>             
        </TableBody>
      </Table>

        
    );
}
