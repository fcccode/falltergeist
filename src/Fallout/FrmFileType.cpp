#include "src/Fallout/FrmFileType.h"
#include "src/Fallout/DatFileItem.h"

namespace Falltergeist
{

FrmFileType::FrmFileType(DatFileItem * datFileItem)
{
    _datFileItem = datFileItem;
    _initialized = false;
    _init();
}

FrmFileType::~FrmFileType()
{
    delete [] _directions; _directions = 0;
}

/**
 * Reads data from DatFileItem
 * @brief FrmFileType::_init
 */
void FrmFileType::_init()
{
    if (_initialized) return;
    _datFileItem->seek(0);
    (*_datFileItem) >> _version >> _framesPerSecond >> _actionFrame >> _framesPerDirection;

    _directions = new FrmDirection[6];

    unsigned short i,j;
    for (i = 0; i < 6; i++) (*_datFileItem) >> _directions[i].shiftX;
    for (i = 0; i < 6; i++) (*_datFileItem) >> _directions[i].shiftY;
    for (i = 0; i < 6; i++) (*_datFileItem) >> _directions[i].dataOffset;

    (*_datFileItem) >> _dataSize;
    // Reading directions
    for (i = 0; i < 6; i++)
    {
        // if direction is the same as first
        if (i > 0) if (_directions[i].dataOffset == _directions[0].dataOffset) {
            _directions[i] = _directions[0];
            continue;
        }

        _directions[i].frames = new FrmFrame[_framesPerDirection];
        for (j = 0; j < _framesPerDirection; j++)
        {
            (*_datFileItem) >> _directions[i].frames[j].width;
            (*_datFileItem) >> _directions[i].frames[j].height;
            (*_datFileItem) >> _directions[i].frames[j].dataSize;
            (*_datFileItem) >> _directions[i].frames[j].offsetX;
            (*_datFileItem) >> _directions[i].frames[j].offsetY;
            _directions[i].frames[j].data = new char[_directions[i].frames[j].dataSize];
            for (unsigned int z = 0; z != _directions[i].frames[j].dataSize; ++z)
            {
                (*_datFileItem) >> _directions[i].frames[j].data[z];
            }
            //_datFileItem->skip(_directions[i].frames[j].dataSize);
        }
    }
    _initialized = true;
}

/**
 * Returns version of FRM file
 * @brief FrmFileType::getVersion
 * @return
 */
unsigned int FrmFileType::getVersion()
{
    return _version;
}

/**
 * Sets version of FRM file
 * @brief FrmFileType::setVersion
 * @param version
 */
void FrmFileType::setVersion(unsigned int version)
{
    _version = version;
}

/**
 * Returns frames per second animation rate
 * @brief FrmFileType::getFramesPerSecond
 * @return
 */
unsigned short FrmFileType::getFramesPerSecond()
{
    return _framesPerSecond;
}

/**
 * Sets frames per second animation rate
 * @brief FrmFileType::setFramesPerSecond
 * @param framesNumber
 */
void FrmFileType::setFramesPerSecond(unsigned short framesNumber)
{
    _framesPerSecond = framesNumber;
}

/**
 * Returns frames number for each of directions
 * @brief FrmFileType::getFramesPerDirection
 * @return
 */
unsigned short FrmFileType::getFramesPerDirection()
{
    return _framesPerDirection;
}

/**
 * Sets frames number for each of directions
 * @brief FrmFileType::setFramesPerDirection
 * @param framesNumber
 */
void FrmFileType::setFramesPerDirection(unsigned short framesNumber)
{
    _framesPerDirection = framesNumber;
}

/**
 * Returns number of frame on which action is occurs
 * @brief FrmFileType::getActionFrame
 * @return
 */
unsigned short FrmFileType::getActionFrame()
{
    return _actionFrame;
}

/**
 * Sets number of frame on which action is occurs
 * @brief FrmFileType::setActionFrame
 * @param frameNumber
 */
void FrmFileType::setActionFrame(unsigned short frameNumber)
{
    _actionFrame = frameNumber;
}

/**
 * Returns array of directions
 * @brief FrmFileType::getDirections
 * @return
 */
FrmDirection * FrmFileType::getDirections()
{
    return _directions;
}

/**
 * Sets array of directions
 * @brief FrmFileType::setDirections
 * @param directions
 */
void FrmFileType::setDirections(FrmDirection * directions)
{
    delete [] _directions;
    _directions = directions;
}

/**
 * Returns frames data
 * @brief FrmFileType::getData
 * @return
 */
unsigned char * FrmFileType::getData()
{
    // 62 - frames data offset
    return _datFileItem->getData() + 62;
}

}