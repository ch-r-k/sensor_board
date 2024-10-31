
#include "i2c.hpp"
#include <cassert>

I2c::I2c(std::uint8_t instance)
{
    assert(instance <= 3 && instance > 0);

    address = 0;

    if (instance == 1)
    {
        handler.Instance = I2C1;
    }
    else if (instance == 2)
    {
        handler.Instance = I2C2;
    }
    else if (instance == 3)
    {
        handler.Instance = I2C3;
    }
}

I2c::~I2c() { handler.Instance = nullptr; }

void I2c::Open()
{
    assert(handler.Instance != nullptr || "nullptr");
    assert(open != true || "must be closed");

    if (HAL_I2C_Init(&handler) != HAL_OK)
    {
        assert(false || "Failed to initialize I2C");
    }

    open = true;
}

void I2c::Close()
{
    assert(open == false || "must be open");

    if (HAL_I2C_DeInit(&handler) != HAL_OK)
    {
        assert(false || "Failed to initialize I2C");
    }
}

void I2c::StartWrite(const std::span<std::uint8_t> data)
{
    assert(open && "I2C must be opened before writing");

    HAL_StatusTypeDef status =
        HAL_I2C_Master_Transmit_IT(&handler, address, data.data(), data.size());

    assert(status == HAL_OK && "I2C Write failed");
}

void I2c::StartRead(const std::span<std::uint8_t> data)
{
    assert(open && "I2C must be opened before reading");

    HAL_StatusTypeDef status =
        HAL_I2C_Master_Receive_IT(&handler, address, data.data(), data.size());

    assert(status == HAL_OK && "I2C Read failed");
}

void I2c::Configure(AddressingMode addressingMode)
{
    handler.Init.AddressingMode = static_cast<std::uint32_t>(addressingMode);
}

void I2c::Configure(DualAddressMode dualAddressMode)
{
    handler.Init.DualAddressMode = static_cast<std::uint32_t>(dualAddressMode);
}

void I2c::Configure(GeneralCallMode generalCallMode)
{
    handler.Init.GeneralCallMode = static_cast<std::uint32_t>(generalCallMode);
}

void I2c::Configure(NoStretchMode noStretchMode)
{
    handler.Init.NoStretchMode = static_cast<std::uint32_t>(noStretchMode);
}

void I2c::setAddress(std::uint8_t address) { this->address = address; }


extern "C" void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    I2c::RxISR(hi2c);
}

extern "C" void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    I2c::TxISR(hi2c);
}

