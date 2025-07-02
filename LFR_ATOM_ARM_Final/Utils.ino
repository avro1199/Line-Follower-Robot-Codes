uint16_t readUShort(byte addr)
{
    return (EEPROM.read(addr) << 8) | EEPROM.read(addr + 1);
}

void writeUShort(byte addr, uint16_t data)
{
    EEPROM.write(addr, data >> 8);
    EEPROM.write(addr + 1, data & 0xFF);
}