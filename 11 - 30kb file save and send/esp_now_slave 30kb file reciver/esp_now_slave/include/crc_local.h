uint16_t get_crc16(String crc_str)
{
    FastCRC16 CRC16;
//     CRC8.clearCrc();
    uint8_t payload[crc_str.length()];

    crc_str.getBytes(payload, crc_str.length() + 1);
//     log_message(DEBUG_MSG, "DEBUG_MSG", "PAYLOAD LENGTH:" + sizeof(payload));

//     for (size_t i = 0; i < crc_str.length(); i++) {
//         CRC8.updateCrc(payload[i]);
//     }

    return CRC16.ccitt(payload, sizeof(payload));
}