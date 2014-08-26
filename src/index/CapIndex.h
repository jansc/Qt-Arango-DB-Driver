/********************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2013 Sascha Ludwig Häusler
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy of
 ** this software and associated documentation files (the "Software"), to deal in
 ** the Software without restriction, including without limitation the rights to
 ** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 ** the Software, and to permit persons to whom the Software is furnished to do so,
 ** subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in all
 ** copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 ** FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 ** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 ** IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 ** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *********************************************************************************/

#ifndef CAPINDEX_H
#define CAPINDEX_H

#include "AbstractIndex.h"

#include <QtCore/QString>

namespace arangodb
{
namespace index
{

class CapIndexPrivate;

/**
 * @brief The cap constraint does not index particular attributes of the documents
 * in a collection, but limits the number of documents in the collection to a maximum
 * value. The cap constraint thus does not support attribute names specified in the
 * fields attribute nor uniqueness of any kind via the unique attribute.
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.6
 */
class ARANGODBDRIVERSHARED_EXPORT CapIndex : public AbstractIndex
{
        Q_OBJECT
        Q_PROPERTY(int size READ size WRITE setSize)
    public:
        /**
         * @brief Constructor
         *
         * @param collection
         * @param parent
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        explicit CapIndex(Collection * collection, QObject *parent = 0);

        /**
         * @brief Sets the max number of Document's which
         * can be stored into the Collection
         *
         * @param size
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setSize(int size);

        /**
         * @brief Returns the maximal number of documents
         * which can be stored into the collection
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        int size() const;

        /**
         * @brief Returns the name/type of the index
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual QString name() const Q_DECL_OVERRIDE;

        /**
         * @brief Returns a json representation of the
         * index
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual QByteArray toJson() const Q_DECL_OVERRIDE;

    private:
        Q_DECLARE_PRIVATE(CapIndex)
};

}
}

#endif // CAPINDEX_H