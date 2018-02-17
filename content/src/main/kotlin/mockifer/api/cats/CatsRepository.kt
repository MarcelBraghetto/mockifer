/*
 * =================================================
 * Mockifer License
 * =================================================
 *
 * MIT License
 *
 * Copyright (c) 2017 Marcel Braghetto
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

package mockifer.api.cats

import mockifer.core.framework.FileLoader.loadJsonDataFile
import kotlin.js.Date

class CatsRepository {
    private val cats: MutableList<Cat> by lazy(this::loadCats)

    fun getAllCats(): List<Cat> {
        return cats
    }

    fun getCat(id: String): Cat? = cats.filter { it.id == id }.firstOrNull()

    fun deleteCat(id: String): Boolean {
        val index = cats.indexOfFirst { it.id == id }

        if (index >= 0) {
            cats.removeAt(index)
            return true
        }

        return false
    }

    fun updateCat(cat: Cat): Pair<Cat, Cat>? {
        val index = cats.indexOfFirst { it.id == cat.id }

        if (index >= 0) {
            val result = Pair(cats[index], cat)
            cats[index] = cat
            return result
        }

        return null
    }

    fun createCat(cat: Cat): Cat {
        cat.id = Date().getTime().toString()
        cats.add(cat)
        return cat
    }

    private class CatListDto(@JsName("cats") val cats: Array<Cat>)

    private fun loadCats(): MutableList<Cat> {
        val dto = loadJsonDataFile<CatListDto>("data/internal/cats/all_cats.json") ?: return mutableListOf()
        val cats = dto.cats.toMutableList()
        cats.forEach { Cat.revive(it) }
        return cats
    }
}